#include <iostream>
#include <thread>
#include <functional>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>

#define LIMITE_MAX 10000000

/*tarea llevada a cabo por los hilos */

void operar(int const id_hilo, int limite){
    for(int i=0;i<limite;i++){
        (i+(i-1))+i;
    }
    std::cout  << "El hilo: "<<id_hilo<<"("<<std::this_thread::get_id() << ") finaliza" << std::endl;
}

/* Creacion de hilos y asignación de tareas */
void soloHilos(int num_hilos){

    std::vector<std::thread> v_hilos;
    for(unsigned i=0;i<num_hilos;i++){

        v_hilos.push_back(std::thread(operar,i,LIMITE_MAX));
    }

    std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
}

/*Creacion de varios procesos con hilos asociados */

void procesosConHilos(int num_procesos, int num_hilos){

    int status;

    for(int i = 0; i< num_procesos;i++){

        int id_proceso;

        if((id_proceso = fork())==-1){
            std::perror("Error en la creación del proceso ");
            exit(EXIT_FAILURE);
        }
        else{
            //solo el hijo crea los hilos
            if(id_proceso == 0){
                soloHilos(num_hilos);
                exit(EXIT_SUCCESS);
            }
            else {  //proceso padre
                std::cout << "Proceso: "<<id_proceso << " CREADO"<<std::endl;
            }
        }
    }

    //Espera para cada proceso hijo

    for(int i=0;i<num_procesos;i++){
        wait(&status);
    }
}

int main(int argc, char *argv[]){


    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<float> fsec;


/* ****************
*
*Dos modos: 1- solo hilos
*           2- varios procesos e hilos por proceso
*
* ej. 1 8 --> creacion de un proceso con 8 hilos
* ej. 2 4 2 --> creacion de 4 procesos con dos hilos cada uno
**************** */


    int modo = atoi(argv[1]);
    int n_proceses = atoi(argv[2]);
    int n_threads = 0;

    if(argc > 3){
        n_threads = atoi(argv[3]);
    }

    std::cout << "PID del proceso principal: " << getpid() << std::endl;

    auto t0 = Time::now();
    
    switch(modo){
        case 1:
            soloHilos(n_proceses);
            break;
        default:
            procesosConHilos(n_proceses, n_threads);
            break;
    }

    auto t1 = Time::now();

    fsec segundos = t1-t0;
    std::cout << "Final del programa. Tiempo de ejecucion: " << segundos.count() << std::endl;

    return 0; 

}
