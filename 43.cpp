#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>
#include <condition_variable>

std::condition_variable cv;
std::mutex semaforo;
int turno = -1;

void foo(const int id){

    std::unique_lock<std::mutex> lk(semaforo);

    std::cout << "Hilo "<< id<<" espera turno ...\n";

    cv.wait(lk,[]{return (turno == 1);});
    std::cout << "Hilo "<<id<<" es mi turno!!! El valor de turno es: "<<turno<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main(){
    const int NUM_HILOS = 10;
    std::vector<std::thread> v;

    for(int i=0;i<NUM_HILOS;i++){
        v.push_back(std::thread(foo,i));
    }

    std::cout<<"Hilos creados"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    turno = 1;
    cv.notify_all();

    for_each(v.begin(),v.end(),std::mem_fn(&std::thread::join));

    return 0;
}