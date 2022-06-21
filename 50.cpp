#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <functional>

void print_int(const int id, std::shared_future<int>& fut){

    int x = fut.get();
    std::cout << "Thread "<<id<<" get value: "<<x<<std::endl;
    if(x == id){
        std::cout << "Thread "<<id<<" es mi turno!!!! \n";
    }
}

int main(){

    int turn = -1;

    std::promise<int> prom;
    std::shared_future<int> fut (prom.get_future());

    std::thread th1(print_int,1,std::ref(fut));
    std::thread th2(print_int,2,std::ref(fut));

    std::this_thread::sleep_for(std::chrono::seconds(2));

    turn = (rand()%2)+1;
    std::cout << "Turno "<<turn<<std::endl;

    prom.set_value(turn);

    th1.join();
    th2.join();

    return 0;
}