/**
 *	Author: Juan Felipe Cortes Renteria
 *	Title: Basic Simulation of a Cache Memory
 *	Description: NULL
 *	
 *	
 *	
 *	
*/


#include <stdio.h>
#include <stdlib.h>
#include "src/cache.h"

void cache_size_sweep(cache);
void block_size_sweep(cache);
void associativity_sweep(cache);


// cache <trace filename> <cache_size kB> <cache_block B> <associativity ways> <Optimization (-on) (-off)>
int main(int argc, char* argv[]){
	
	
	
	std::string opFlag = "-off";
	std::string sweepFlag = "";
	std::string tracefilename = "";
	int size;
	int line;
	int ways;
	if(argc > 2 && argc <= 4){		
		tracefilename = argv[1];
		sweepFlag = argv[2];
		if(argc == 4)
			opFlag = argv[3];
	}else if(argc < 5){
		std::cout << "Not enough arguments..." << std::endl << std::endl;
		exit(1);
	}else{
		tracefilename = argv[1];
		size = atoi(argv[2]);
		line = atoi(argv[3]);
		ways = atoi(argv[4]);
		opFlag = "-off";
		if(argc == 6)
			opFlag = argv[5];
	}
	
	cache myCache;//(size, line, ways);
	myCache.setTrace(tracefilename);
	myCache.setOptimization(opFlag);
	//myCache.getInfo();
	//myCache.startSimulation(tracefilename);
	//myCache.getData();
	if(sweepFlag == "-sweep"){
		cache_size_sweep(myCache);
		block_size_sweep(myCache);
		associativity_sweep(myCache);
	}else{
		myCache.setup(size, line, ways);
		myCache.getInfo();
		myCache.startSimulation();
		myCache.getData();
	}
	return 0;
}


void cache_size_sweep(cache size_sweep){
	std::cout << std::endl;
	std::cout << "Cache Size -> (32kB, 64kB, 128kB)" << std::endl;
	int powValue;
	for(int i = 0; i < 3; i++){
		powValue = 32*pow(2, i);
		std::cout << "Run: " << i + 1 << "/3" << std::endl;
		size_sweep.setup(powValue, 64, 16);
		size_sweep.startSimulation();
		size_sweep.getRates();
	}
}

void block_size_sweep(cache line_sweep){
	std::cout << std::endl;
	std::cout << "Block Size -> (32B, 64B, 128B)" << std::endl;
	int powValue;
	for(int i = 0; i < 3; i++){
		powValue = 32*pow(2, i);
		std::cout << "Run: " << i + 1 << "/3" << std::endl;
		line_sweep.setup(32, powValue, 8);
		line_sweep.startSimulation();
		line_sweep.getRates();
	}
}

void associativity_sweep(cache ways_sweep){
	std::cout << std::endl;
	std::cout << "Ways Size -> (4, 8, 16)" << std::endl;
	int powValue;
	for(int i = 0; i < 3; i++){
		powValue = 4*pow(2, i);
		std::cout << "Run: " << i + 1 << "/3" << std::endl;
		ways_sweep.setup(32, 64, powValue);
		ways_sweep.startSimulation();
		ways_sweep.getRates();
	}
	
}




