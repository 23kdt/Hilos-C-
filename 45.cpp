#include <iostream>
#include <thread>
#include <functional>
#include <exception>
#include <future>

void get_int(std::promise<int>& prom){

    int x;
    std::cout << "Por favor, introduce por teclado un valor entero: ";
    std::cin.exceptions (std::ios::failbit);    //excepción en la entrada de datos

    try {
        std::cin >> x;
        prom.set_value(x);
    }
    catch(std::exception&){
        prom.set_exception(std::current_exception());
    }
}

void print_int(std::future<int>& fut){

    try {
        int x = fut.get();
        std::cout << "Valor: "<< x<< "\n";

    }
    catch(std::exception& e){
        std::cout << "[exception caught: "<<e.what()<<"]\n";
    }
}

int main(){
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread th1(print_int,std::ref(fut));
    std::thread th2(get_int,std::ref(prom));

    th1.join();
    th2.join();

    return 0;
}
