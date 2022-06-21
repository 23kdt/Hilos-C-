#include <mutex>
#include <iostream>
#include <thread>

bool flag;
std::mutex m;

void esperaPorFlag(){
    std::unique_lock<std::mutex> lk(m);
    while(!flag){
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }
}