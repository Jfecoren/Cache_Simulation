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

// cache <trace filename> <cache_size kB> <cache_block B> <associativity ways> <Optimization (-on) (-off)>
int main(int argc, char* argv[]){
	if(argc < 5){
		std::cout << "Not enough arguments..." << std::endl << std::endl;
		exit(1);
	}
	std::string tracefilename = argv[1];
	int size = atoi(argv[2]);
	int line = atoi(argv[3]);
	int ways = atoi(argv[4]);
	std::string opFlag = "-off";
	if(argc == 6)
		opFlag = argv[5];
	
	cache myCache(size, line, ways);
	myCache.setOptimization(opFlag);
	myCache.getInfo();
	myCache.startSimulation(tracefilename);
	myCache.getData();
	
	
	return 0;
}