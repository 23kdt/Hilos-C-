#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>

std::mutex semaforo;
std::queue<int> cola;
std::condition_variable cv;

void producir(){

    for(unsigned i=0;i<100;i++){
        int numero = (rand()%100)+1;
        std::lock_guard<std::mutex> lk(semaforo);
        cola.push(numero);

        std::cout << "Se genera un nuevo valor: "<< numero << " en cola \n";
        cv.notify_one();
    }
}

void consumir(){

    int numero;

    while(true){
        std::unique_lock<std::mutex> lk (semaforo);

        std::cout << "¿Hay elementos en la cola? \n";
        cv.wait(lk,[] {return !cola.empty();});

        //dato disponible en la cola
        numero = cola.front();
        cola.pop();
        lk.unlock();

        std::cout << "Se ha extraido el numero "<<numero<< " de la cola \n";
    }
}

int main(){

    std::thread t_consumidor(consumir);
    std::thread t_productor(producir);

    t_consumidor.join();
    t_productor.join();

    return 0;
}