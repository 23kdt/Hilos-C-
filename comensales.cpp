#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::mutex s_tenedor;
std::mutex s_cuchillo;

void comer(int comensal){

    s_tenedor.lock();
    s_cuchillo.lock();

    std::cout << "El comensal " << comensal << " esta comiendo \n";

    s_tenedor.unlock();
    s_cuchillo.unlock();

}

int main(){

    std::thread c1 (comer,1);
    std::thread c2 (comer,2);
    
    c1.join();
    c2.join();
}