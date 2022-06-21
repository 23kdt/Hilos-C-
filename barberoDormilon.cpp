#include <iostream>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>


std::mutex m;
std::mutex mu;
std::condition_variable cv;

int asientos = 4;
bool listo = false;
bool processed = false;
bool salaLlena = false;
bool haircutdone = false;
static const int num_threads = 10;
std::queue <int> q;

void getSleep() {

    std::unique_lock<std::mutex> lk(m);
    std::cout << "El barbero se ha dormido \n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    cv.wait(lk,[] {return listo;});

    processed = true;

    lk.unlock();
    cv.notify_one();
}

void getHairCut(int i){

    mu.lock();
    haircutdone = false;

    if(listo){
        std::cout << "Cortando el pelito \n";
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
    haircutdone = true;
    mu.unlock();

}

void salaEspera(int i){

    if(q.size() <= (size_t)asientos) {

        std::cout << "Sala de espera --> "<<i<<"\n";
        q.push(i);

        if(listo){
            getHairCut(i);
            q.pop();
        }

    }
    else {
        std::terminate();
        std::cout << "Cliente "<<i<<" se ha ido \n";
    }
}

int main(){

    std::thread barber(getSleep);

    //envio de datos al hilo barbero
    {
    std::lock_guard<std::mutex> lk(m);
    listo = true;
    }
    cv.notify_one();


    //espera al barbero
    {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk,[] {return processed;});
    }

    std::thread clientes[num_threads];
    unsigned int n = std::thread::hardware_concurrency();

    for(int i=1;i<n;++i){
        clientes[i] = std::thread(salaEspera,i);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    for(auto &cli: clientes){
        cli.join();
    }

    return 0;

}
