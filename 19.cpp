#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void hacerAlgo(unsigned id);

void f(){

    std::vector<std::thread> vhilos;
    for(unsigned i=0;i<50;i++)
        vhilos.push_back(std::thread(hacerAlgo,i));

    std::for_each(vhilos.begin(),vhilos.end(),std::mem_fn(&std::thread::join));
}

