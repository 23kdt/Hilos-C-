#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <algorithm>
#include <condition_variable>
#include <atomic>
#include <vector>


#define nfilosofos 5

struct Palillos{

    public:
        Palillos(){;};
        std::mutex m;
    };


    void eat (Palillos &left, Palillos &right, int i){

    std::unique_lock<std::mutex> ilk(left.m);
    std::unique_lock<std::mutex> dlk(right.m);

    std::cout << "El filosofo "<< i<< " está comiendo \n";

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "El filosofo "<<i<<" ha terminado de comer \n";
    };

int main(){

    Palillos pal[nfilosofos];

    std::thread filosofos[nfilosofos];


    filosofos[0]= std::thread (eat,std::ref(pal[0]),std::ref(pal[nfilosofos-1]),0);

    for(int i=1;i<nfilosofos;i++){
        std::cout << "Filosofo "<<i<<" está leyendo... \n";
        filosofos[i]=std::thread(eat,std::ref(pal[0]),std::ref(pal[nfilosofos-1]),i);
    }

    for(auto &fil:filosofos){
        fil.join();
    }

    return 0;
}


