#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>

class MiClase {

public:
    int _n_hilo;
    int _aleatorio;

    MiClase(int);
    void foo();

};

MiClase::MiClase (int id){
    this -> _n_hilo = id;
}


void MiClase::foo(){
    std::cout << "Hilo "<< this->_n_hilo << "["<<std::this_thread::get_id() <<"]"<< "funcionando ..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    this ->_aleatorio = rand()%100;

    std::cout << "Hilo "<<this->_n_hilo << "["<<std::this_thread::get_id<<"] FINALIZADO" <<std::endl;
}

int main(){

    std::vector<MiClase> v_objetos;
    std::vector<std::thread> v_hilos;

    for(int i=0;i<5;i++) v_objetos.push_back(MiClase(i));

    for(int i = 0;i<5;i++){
        v_hilos.push_back(std::thread(&MiClase::foo, &v_objetos[i]));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    //esperamos por cada hilo
    
    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));

    for(int i=0;i<5;i++){
        std::cout << "Hilo :"<<v_objetos[i]._n_hilo << "Aleatorio: "<<v_objetos[i]._aleatorio << std::endl;
    }

    std::cout << "Fin del programa" <<std::endl;

    return 0;

}