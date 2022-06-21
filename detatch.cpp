#include <iostream>
#include <thread>
#include <chrono>

void nieto() {

    std::this_thread::sleep_for(std::chrono::seconds(20));
    std::cout << "Hilo nieto finaliza "<<std::endl;
}

void hijo(){

    std::thread h_nieto(nieto);
    h_nieto.detach();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Hilo hijo finaliza " << std::endl; 
}

int main(){
    std::thread h_hijo(hijo);
    h_hijo.detach();
    std::this_thread::sleep_for(std::chrono::seconds(8));
    std::cout << "Fin de main " << std::endl;

    return 0;

}