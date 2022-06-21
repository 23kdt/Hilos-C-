#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <algorithm>
#include <queue>
#include <vector>
#include <condition_variable>

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

void SemCounter::block() {
    mutex_block.lock();
}

void SemCounter::unblock(){
    mutex_block.unlock();
}

void SemCounter::wait(){

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


#define Buffer_size 5

SemCounter vacios(Buffer_size);
SemCounter llenos(0);
std::mutex m;

std::condition_variable cv;
std::queue<int> cola;

void producir(){
    int i=0;
    do{
        /*El hilo esperara hasta que el contador de vacíos tenga hueco */
        vacios.wait();
        m.lock();
        int numero = (rand()%100)+1;
        cola.push(numero);
        std::cout << "Se genera un nuevo valor: "<<numero<< " en la cola \n";

        /*Desbloqueamos el semaforo e incrementamos el contador de llenos */
        m.unlock();
        llenos.signal();
    }
    while(i++ < 100);
}

void consumir(){
    int numero;
    do{
        llenos.wait();
        m.lock();

        numero = cola.front();
        cola.pop();
        std::cout << "Se ha extraido el numero "<<numero<<" de la cola \n";

        vacios.signal();
        m.unlock();

    }
    while(true);
}

int main(){

    std::thread consumidor(consumir);
    std::thread productor(producir);

    consumidor.join();
    productor.join();

    return 0;

}


