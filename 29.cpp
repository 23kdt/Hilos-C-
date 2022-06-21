#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

int numero = 0;     //variable global protegido con semaforo
std::mutex semaforo;

void incremento(int id){

    for(int i=0;i<3;i++){
        semaforo.lock();
        ++numero;
        std::cout << id << "-->" << numero << "\n";
        semaforo.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


int main(){

    std::thread h1(incremento,0);
    std::thread h2(incremento,1);

    h1.join();
    h2.join();
}