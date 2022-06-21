#include <iostream>
#include <thread>

void cambiaValor(int x){

    x = 2475;
}

int main(){

    int v = 7;
    std::cout << "Antes de la ejecucion del hilo v= "<<v<<std::endl;

    std::thread t(cambiaValor,v);
    t.join();
    std::cout << "Despues de la ejecucion del hilo v=" <<v<<std::endl;

    return 0;


}