#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

class SemCounter {
    private:
        int value;
        std::mutex mutex_;
        std::mutex mutex_block;

        void block();
        void unblock();

    public:
        SemCounter(int value);
        void wait();
        void signal();
        int getValue();
};

SemCounter::SemCounter(int v): value(v){};

void SemCounter::block() {mutex_block.lock();}
void SemCounter::unblock() {mutex_block.unlock();}

void SemCounter::wait() {

    mutex_.lock();
    if(--value <=0){
        mutex_.unlock();
        block();
        mutex_.lock();
    }
    mutex_.unlock();
}

void SemCounter::signal(){

    mutex_.lock();
    if(++value <=0){
        unblock();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    mutex_.unlock();
}

int SemCounter::getValue(){
    return value;
}




/********* VARIABLES COMPARIDAS ****/

int const MAX_THREADS = 3;
int const ARRAY_SIZE_IDS = 15;

SemCounter sem_ (MAX_THREADS);
int id_hilos[ARRAY_SIZE_IDS];


//Muestra los identificadores de los hilos activos
void mostrarHilosActivos(){
    std::cout << "[";
    for(int i=0;i<ARRAY_SIZE_IDS;i++){
        std::cout << id_hilos[i];
        if(i<ARRAY_SIZE_IDS-1){
            std::cout << " | ";
        }
    }
    std::cout << "]\n";
}

/* Uso del semaforo contador. Funcion ejecutada por los hilos */

void foo(int id){

    sem_.wait();
    id_hilos[ARRAY_SIZE_IDS]= id+1;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    id_hilos[id%ARRAY_SIZE_IDS] = -1;
    sem_.signal();
}

int main(){

    std::thread threads[ARRAY_SIZE_IDS];

    for(int i=0;i<ARRAY_SIZE_IDS;i++){
        threads[i] = std::thread(foo,i);
    }

    while(1){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Valor SEMAFORO: "<< sem_.getValue() <<" :: ";
        mostrarHilosActivos();
    }

    for(int i=0;i<ARRAY_SIZE_IDS;i++){
        threads[i].join();
    }

}

