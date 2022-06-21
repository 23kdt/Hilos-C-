#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <algorithm>

#define LIMITE 4000000
#define N_HILOS 5


std::atomic<long> n_suma_acumulada (0);

void Suma(std::vector<int> const& v_numeros, int li, int ls){

    for(int i=li;i<ls;i++){
        n_suma_acumulada += v_numeros[i];
    }
}


int main(){

    std::vector<int> v_numeros;
    std::vector<std::thread> v_hilos;

    int size_task = LIMITE/N_HILOS;

    long sum = 0;

    for(int i=0;i<LIMITE;i++){
        v_numeros.push_back(rand());
        sum += v_numeros[i];
    }

    std::cout << "La suma acumulada es: " << sum << std::endl;

    for(int i=0;i<N_HILOS;i++){
        int begin = i*size_task;
        int end = (begin + size_task)-1;
        if(i == N_HILOS-1){
            end = LIMITE -1;
        }

        v_hilos.push_back(std::thread(Suma,v_numeros,begin,end));
    }

    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));

    std::cout << "La suma acumulada por los hilos es: "<< n_suma_acumulada.load() << std::endl;

    return 0;
}