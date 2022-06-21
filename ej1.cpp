#include <iostream>
#include <thread>

void saludo(){
    std::cout << "Hola mundo \n";

}

int main(){

    std::thread hilo(saludo);

    hilo.join();
}