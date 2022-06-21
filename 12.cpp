#include <iostream>
#include <thread>
#include <chrono>

class MiClase {

private:
    int x;
public:

    MiClase(int x){
        this->x = x;
    }


    void operator () () {

        std::cout << "Hilo produciendo incremento ..." << std::endl;
        incremento();
    }

    void incremento(){
        this->x++;
    }

    int getX(){
        return this->x;
    }
};


int main(){

    MiClase m(0);

    std::thread mi_hilo(std::ref(m));
    mi_hilo.join();
    std::cout << "El valor de x en m es: " << m.getX() << std::endl;
    m.incremento();
    std::cout << "El valor de x en m es: " << m.getX() << std::endl;

    return (EXIT_SUCCESS);

}