#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <functional>
#include <algorithm>
#include <future>
#include <queue>


struct Peticion{
    private:
        int id_peticion;
    public:
        Peticion(int id){
            this->id_peticion = id;
        };
        
        int getId(){
            return id_peticion;
        }
        void setValor(int x){
            this->id_peticion = x;
        }  
};

std::mutex s_peticion;

std::condition_variable cv;

std::queue<Peticion> cola_peticion;

void cliente(std::future<Peticion>& fut){

    int numero = (rand()%100)+1;    //numero que será el id de la peticion
    Peticion peticion(numero);
    std::unique_lock<std::mutex> lk(s_peticion);
    cola_peticion.push(peticion);
    std::cout << "El cliente ha realizado una petición con id" << numero<< std::endl;
    cv.notify_one();

    lk.unlock();
    Peticion respuesta = fut.get();
    std::cout<< "La respuesta del servidor es "<<respuesta.getId() << std::endl;
}


void servidor(std::promise<Peticion>& prom){ 

    std::unique_lock<std::mutex> lk(s_peticion);
    cv.wait(lk,[] {return !cola_peticion.empty();});
    std::cout << "El servidor ha recibido una peticion \n";
    Peticion respuesta = cola_peticion.front();
    cola_peticion.pop();
    lk.unlock();
    if(respuesta.getId() > 50){
        respuesta.setValor(1);
    }
    else{
        respuesta.setValor(0);
    }
    prom.set_value(respuesta);
    std::cout<<"El servidor ha enviado la respuesta \n";
}

int main(){

    std::promise<Peticion> prom;
    std::future<Peticion> fut = prom.get_future();

    std::thread server(servidor,std::ref(prom));
    std::thread client(cliente,std::ref(fut));

    server.join();
    client.join();
    
    return 0;

}

