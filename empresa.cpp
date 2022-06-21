/*
Codifique un programa en C++ que
permita la comunicación y sincronización entre varios hilos con el uso de objetos promise y future.
Sea una empresa cuya localización está distribuida en dos edificios diferentes y se desea llevar a
cabo un control de los gastos de energía. Los gastos mensuales de energía, están almacenados en
ficheros; uno por cada edificio. Cada archivo contiene registrado el gasto de luz diario en una línea
diferente. Por tanto, si un mes consta de 30 días, el archivo constará de 30 líneas con valores
numéricos que representan el gasto de cada uno de los días.
Se solicita crear un hilo que calcule el gasto mensual del primer edificio, un segundo hilo que
calcule de forma paralela el gasto de energía en el segundo edificio, y un tercer hilo que permanezca
a la espera del cálculo de estos dos valores. Cuando el tercer hilo, reciba estos valores los sumará
para determinar el gasto total de energía en la empresa y lo mostrará por pantalla.
Por otro lado, un cuarto y quinto hilos deben encargarse, de forma paralela a los dos primeros hilos,
de determinar la media de gasto de energía por día en cada uno de los edificios. Finalmente, estos
datos deben ser enviados al tercer hilo, que de nuevo debe fusionar los datos para calcular la media
de gasto diario en toda la empresa, y lo muestre por pantalla. */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>



std::vector<int> leerfichero(const char *fichero){

    std::ifstream input(fichero);

    if(!input.is_open()){
        std::cout << "Error al abrir el fichero\n";
        exit(EXIT_FAILURE);
    }

    int valor;
    std::vector<int> datos;
    while(input >> valor){
        datos.push_back(valor);
    }

    return datos;
}

void calcularSuma(const std::vector<int> v, std::promise<int>& prom){
    int suma=0;

    for(auto x: v){
        suma += x;
    }

    prom.set_value(suma);
}

void calcularMedia(const std::vector<int> v, std::promise<int>& prom){
    int suma=0;

    for(auto x: v){
        suma += x;
    }

    double media = (suma/sizeof(v));

    prom.set_value(media);   
}


void muestraVector(const std::vector<int>& v){

    for(auto x:v){
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(){

    int v1,v3;
    double v2,v4;

    /*Creación de promises y futures */

    std::promise <int> prom1;
    std::promise <int> prom2;
    std::promise <int> prom3;
    std::promise <int> prom4;

    std::future<int> fut1 = prom1.get_future();
    std::future<int> fut2 = prom2.get_future();
    std::future<int> fut3 = prom3.get_future();
    std::future<int> fut4 = prom4.get_future();

    /*Lectura de ficheros */

    std::vector<int> datos1 = leerfichero("fichero1.txt");
    std::vector<int> datos2 = leerfichero("fichero2.txt");

    /*Creación de hilos */

    std::thread h1(calcularSuma,datos1,std::ref(prom1));
    std::thread h2(calcularMedia,datos1, std::ref(prom2));
    std::thread h3(calcularSuma,datos2,std::ref(prom3));
    std::thread h4(calcularMedia,datos2,std::ref(prom4));

    /*Obtenemos los valores mediante los future */
    v1 = fut1.get();
    v2 = fut2.get();
    v3 = fut3.get();
    v4 = fut4.get();

    /*Esperamos a la finalización de los hilos */
    h1.join();
    h2.join();
    h3.join();
    h4.join();

    std::cout << " Los valores obtenidos son: \n"<< v1<<"\n"<<v2<<"\n" <<v3<<"\n"<<v4<<std::endl;

    return 0;


}

