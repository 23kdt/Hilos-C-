#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <algorithm>
#include <condition_variable>
#include <functional>

std::condition_variable cv;
std::mutex clientes;
std::mutex manager;
int turno = -1;

int foo(const int id){

    std::unique_lock<std::mutex> lk (clientes);

    std::cout << "Hilo "<<id<<" espera turno... \n";
    cv.wait(lk, [id] {return (turno==id);});
    std::cout << "Hilo "<<id<<" es mi turno !!!! \n";
    manager.unlock();
}

int main(){

    const int NUM_HILOS = 10;
    std::vector<std::thread> v;
    manager.lock();

    for(int i=0;i<NUM_HILOS;i++){
        v.push_back(std::thread(foo,i));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    for(int i=0;i<NUM_HILOS;i++){

        std::cout <<"El turno es del hilo: "<<i<<std::endl;
        turno = i;
        cv.notify_all();
        manager.lock();
    }
    for_each(v.begin(),v.end(),std::mem_fn(&std::thread::join));

    return 0;
}
