#include <iostream>
#include <mutex>
#include <chrono>
#include <functional>
#include <algorithm>
#include <condition_variable>
#include <thread>
#include <atomic>

using namespace std;

int main(){

    const int nfilosofos = 5;

    struct Palillos {

        public:
            Palillos(){;}
            std::mutex m;
    };

    auto eat = [] (Palillos &left_pal, Palillos &right_pal, int id){


        std::unique_lock<std::mutex> ilk(left_pal.m);
        std::unique_lock<std::mutex> dlk(right_pal.m);

        cout << "Filosofo "<<id<<" está comiendo \n";

        std::this_thread::sleep_for(std::chrono::seconds(2));

        cout << "Filosofo "<<id<<" ha terminado de comer \n";
    };

    //crear palillos

    Palillos pal[nfilosofos];

    //crear filosofos
    std::thread filosofos[nfilosofos];

    //Los filosofos empiezan a leer
    cout << "Filosofo "<< (0+1) << " está leyendo .." << endl;
    filosofos[0] = std::thread(eat,std::ref(pal[0]),std::ref(pal[nfilosofos-1]),(0+1));

    for(int i=1;i<nfilosofos;i++){
        cout << "Filosofo "<<(i+1)<< " está leyendo.."<<endl;
        filosofos[i] = std::thread(eat,std::ref(pal[i]),std::ref(pal[i-1]),(i+1));
    }

    for(auto &fil: filosofos){
        fil.join();
    }

}
