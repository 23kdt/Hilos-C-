#include <iostream>
#include <thread>

class Punto {

private:
    int x;
    int y;

public:
    Punto(int x, int y){
        this->x = x;
        this->y = y;
    }

    Punto operator +(Punto &p){
        return Punto(this->x + p.x, this->y + p.y);
    }

    void mostrarPunto(){
        std::cout << "(" << this->x <<","<< this->y << ")";

    }
};


int main(){
    Punto p1(3,4);
    Punto p2(6,1);

    Punto p3 = p1 + p2;

    p1.mostrarPunto();

    std::cout << " + ";

    p2.mostrarPunto();

    std::cout << " = ";

    p3.mostrarPunto();

    std::cout << std::endl;

    return 1;
}