#pragma once
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <chrono>


enum keys{
	EXIT_KEY = 0,
	EMPTY_KEY = -1,
};

struct cache_block{
	bool valid;
	int tag;
	//int offset;
};

struct cache_base{
	std::vector<int> lru_policy;
	std::vector<cache_block> block;
};

struct trace_base{
	bool lost;
	int address;
};


class cache{
	private:
		// expected cache data
		unsigned int hit = 0;
		unsigned int miss = 0;
		unsigned int replace = 0;
		unsigned int load_hit = 0;
		unsigned int store_hit = 0;
		unsigned int load_miss = 0;
		unsigned int store_miss = 0;
		
		unsigned int op_hit = 0;
		unsigned int op_miss = 0;
		// cache characteristics
		unsigned int cache_size = 0;
		unsigned int cache_line = 0;
		unsigned int cache_ways = 0;
		// cache data division
		unsigned int index_size;
		unsigned int index_bits;
		unsigned int offset_bits;
		unsigned int tag_bits;
		// cache simulated structure
		std::vector<int> lru_base;
		std::vector<trace_base> trace;								// holds lecture from trace file
		bool optimization = false;
		
		// private functions
		std::vector<int> lru(std::vector<int>, int);				// manipulate lru policy
		int decimalAddress(std::string);
		int bitExtract(int, int, int);
		void setup_error(void);										// error is characteristics input is wrong
		void lruState(std::vector<int>);
		void simulation(std::vector<trace_base>);
		
	public:
		cache();
		cache(int, int, int);
		~cache();
		// cache functions
		void setup(int, int, int);
		void getInfo(void);
		void getData(void);
		void setTrace(std::string);
		void setOptimization(std::string);
		void printTrace(int);
		void startSimulation(void);
		void startSimulation(std::string);
		void startSimulation(std::vector<trace_base>);
		
		
};