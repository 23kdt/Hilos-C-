#include <iostream>
#include <thread>
#include <future>

int tareaPesada();
int hacerotracosa();

int main(){

    std::future<int> valor_generado = std::async(tareaPesada);
    hacerotracosa();
    std::cout << "El valor generado es "<<valor_generado.get()<<std::endl;

    return 0;

}