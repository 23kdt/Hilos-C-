#include <iostream>
#include <thread>
#include <future>
#include <functional>


void imprimir_entero(std::future<int>& fut){

    int x = fut.get();
    std::cout << " Valor: "<<x<<std::endl;
}

int main(){

    std::promise<int> prom;

    std::future<int> fut = prom.get_future();   //creación de future y asociación

    std::thread th1(imprimir_entero,std::ref(fut));

    prom.set_value(10);

    th1.join();

    return 0;


}