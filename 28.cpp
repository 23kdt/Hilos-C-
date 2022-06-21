#include <iostream>
#include <mutex>
#include <thread>
#include <list>
#include <chrono>
#include <algorithm>

#define NELEMENTOS 20

std::list<int> lista; //lista compartida por los hilos
std::mutex semaforo;

void incluirElementosLista(int v){

    //uso de semaforo. acceso protegido a la lista

    std::lock_guard<std::mutex> guard(semaforo);
    lista.push_back(v);
}

std::list<int>::iterator buscaElementoLista(int valor){

    //Uso de semaforo. Acceso protegido a la lista

    std::lock_guard<std::mutex> guard(semaforo);
    return std::find(lista.begin(),lista.end(),valor);
}

void completarLista(){

    int valor;

    for(unsigned i=0;i<NELEMENTOS;i++){

        valor = (rand()%20)+1;
        incluirElementosLista(valor);
        std::cout << "Se incluye en la lista el valor: "<<valor<<"\n";
        //dormimos el hilo durante 2ms
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void buscar() {

    int valor;
    std::list<int>::iterator it;

    for(unsigned i=0;i< NELEMENTOS;i++){
        //busca en la lista un valor aleatorio entre 1 y 20
        valor = (rand()%20)+1;
        std::cout << "Se busca en la lista el valor: "<<valor<<"\n";

        it = buscaElementoLista(valor);

        if(it != lista.end()){
            std::cout << " ********** Valor encontrado en: "<<std::distance(lista.begin(),it) << "*********\n";
        }

        //dormimos el hilo durante 10 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
}

int main(){

    //hilo1 crea datos en la lista
    //hilo2 busca datos en la lista
    std::thread hilo1(completarLista);
    std::thread hilo2 (buscar);

    hilo1.join();
    hilo2.join();

    return 0;
}



