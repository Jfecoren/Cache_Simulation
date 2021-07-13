/**	Cache Class
 *	https://www.pluralsight.com/blog/software-development/how-to-measure-execution-time-intervals-in-c--
 *	https://www.educba.com/c-plus-plus-read-file/
 *	https://www.geeksforgeeks.org/csv-file-management-using-c/
 *	https://stackoverflow.com/questions/47866710/vector-of-vectors-of-structs
 *	https://www.tutorialspoint.com/cplusplus/
 *	
 */

#include "cache.h"

#define ADDRESS_BIT_SIZE 32
#define ADDRESS_HEX_SIZE 8
#define DELIMITER ' '


cache::cache(){
	
}

cache::cache(int size, int line, int ways){
	setup(size, line, ways);
}

cache::~cache(){
	
}


void cache::setup(int size, int line, int ways){
	
	cache_size = size;
	cache_line = line;
	cache_ways = ways;
	
	setup_error();
	
	index_size = cache_size*1024/(cache_line*cache_ways);
	index_bits = log2(index_size);
	offset_bits = log2(cache_line);
	tag_bits = ADDRESS_BIT_SIZE - index_bits - offset_bits;
	
	//lru_base = std::vector<int>(cache_ways);
	for(int l = 0; l < cache_ways; l++) lru_base.push_back(l);
	//lruState(lru_base);
}


void cache::startSimulation(void){
	if(trace.size() == 0) std::cout << "Tracefile not loaded" << std::endl;
	else simulation(trace);
}


void cache::startSimulation(std::string filename){
	setTrace(filename);
	simulation(trace);
}


void cache::startSimulation(std::vector<trace_base> tracefile){
	simulation(tracefile);
}


void cache::getInfo(void){
	std::cout << std::endl;
	std::cout << "For a " << cache_size << "kB cache, " << cache_ways << " ways & block size of " << cache_line << "B:" << std::endl; 
	
	std::cout << "Index size: " << index_size << std::endl;
	std::cout << "Index bits: " << index_bits << std::endl;
	std::cout << "Tag bits: " << tag_bits << std::endl;
	std::cout << "Offset bits: " << offset_bits << std::endl << std::endl;
	
}



void cache::getData(void){
	std::cout << "Load  hits: " << load_hit << std::endl;
	std::cout << "Store hits: " << store_hit << std::endl;
	std::cout << "Total hits: " << hit << std::endl;
	std::cout << "Load  miss: " << load_miss << std::endl;
	std::cout << "Store miss: " << store_miss << std::endl;
	std::cout << "Total misses: " << miss << std::endl;
	std::cout << "Total replaces: " << replace << std::endl;
	if(optimization){
		double accuracy = (double)op_hit/((double)(op_hit + op_miss));
		std::cout << std::endl;
		std::cout << "Optimization: Way Prediction " << std::endl;
		std::cout << "Way prediction hits: " << op_hit << std::endl;
		std::cout << "Way prediction miss: " << op_miss << std::endl;
		std::cout << "Way prediction accuracy: " << accuracy << std::endl;
	}
	
	std::cout << std::endl;

}


void cache::getRates(void){
	double rate = (double)hit/((double)(hit+miss));
	std::cout << "Total hits: " << hit << std::endl;
	std::cout << "Total misses: " << miss << std::endl;
	std::cout << "Total replaces: " << replace << std::endl;
	std::cout << "Hit rate: " << rate << std::endl;
	if(optimization){
		double accuracy = (double)op_hit/((double)(op_hit + op_miss));
		std::cout << std::endl;
		std::cout << "Optimization: Way Prediction " << std::endl;
		std::cout << "Way prediction hits: " << op_hit << std::endl;
		std::cout << "Way prediction miss: " << op_miss << std::endl;
		std::cout << "Way prediction accuracy: " << accuracy << std::endl;
	}
}


void cache::setTrace(std::string file){
	trace.clear();
	
	std::ifstream traceout;
	std::string insAddress;
	bool insType;
	std::cout << "Loading tracefile to system..." << std::endl;
	traceout.open(file);
	if(!traceout){
		std::cout << "Tracefile not found!" << std::endl;
		exit(1);
	}
	
	int saves = 0;
	while(!traceout.eof() ){
		std::string traceline;
		getline(traceout, traceline, DELIMITER);
		
		if(saves == 1){
			if(traceline == "0") insType = false;
			if(traceline == "1") insType = true;
			//std::cout << insType << std::endl;
		}
		if(saves == 2){
			insAddress = traceline;
			//std::cout << decimalAddress(insAddress) << std::endl;
		}
		if(saves == 3){
			trace.push_back({insType, decimalAddress(insAddress)});
			saves = 0;
		}
		//std::cout << traceline << std::endl;
		saves++;
	}
	
	traceout.close();
	
}


void cache::setOptimization(std::string flag){
	if(flag == "-ON" || flag == "-on") optimization = true;
	else if(flag == "-OFF" || flag == "-off") optimization = false;
	else{
		std::cout << "Optimization flag error" << std::endl;
		exit(1);
	}
	
}



void cache::printTrace(int printlines){
	if(printlines == -1) printlines = trace.size();
	for(int tr = 0; tr < printlines; tr++)
		std::cout << trace[tr].lost << " " << trace[tr].address << std::endl;
	
}

// PRIVATE FUNCTIONS //
void cache::setup_error(void){
	if(cache_size == 0 || cache_line == 0 || cache_ways == 0){
		std::cout << "Wrong cache setup..." << std::endl;
		system("pause");
		exit(1);
	}
}


std::vector<int> cache::lru(std::vector<int> lru_vector, int used_way){
	std::vector<int> index(cache_ways);												// index with size
	std::vector<int> temp(cache_ways);													// temporal array
	int way_index = 0;													// position (index) of the least recently used way (LRU Way)
	for(int i = 0; i < cache_ways; i++){								
		index[i] = i;													// fill actual index
		if(lru_vector[i] == used_way) way_index = i;					// finds LRU Way position
	}

	for(int i = 0; i <= way_index; i++){								// re-order index for new order
		index[i] = index[i] - 1;
	}
	index[0] = way_index;
	
    for (int i = 0; i < cache_ways; i++)								// use temp array to set new order
        temp[i] = lru_vector[index[i]];
	
	return temp;	
	/* //DEBUG WHILE
	while(1){
		std::cout << "Ingrese el way utilizado para asociatividad: " << 4 << std::endl;
		std::cin >> way_to_use;
		if(way_to_use == -1) break;
		lru_order = myCache.lru(lru_order, way_to_use);
		for(int i = 0; i < 4; i++) std::cout << lru_order[i] << " ";
		std::cout << std::endl;
	}
	*/
	
	
}


int cache::decimalAddress(std::string hexNumber){
	return std::stol(hexNumber, nullptr , 16);
}


int cache::bitExtract(int number, int k, int p){
    return (((1 << k) - 1) & (number >> (p - 1)));
	/*
    cout << "The offset number: " <<
        bitExtract(tracefile[0], offset_bits, 1) << endl;
    cout << "The index number: " <<
        bitExtract(tracefile[0], index_bits, offset_bits + 1) << endl;
	cout << "The tag number is: " <<
        bitExtract(tracefile[0], tag_bits, index_bits + 1) << endl << endl;
	*/
}


void cache::lruState(std::vector<int> lru_vector){
	for(int p = 0; p < cache_ways; p++) std::cout << lru_vector[p] << " ";
	std::cout << std::endl;
}


void cache::simulation(std::vector<trace_base> tracefile){
	
	// expected cache data
	hit = 0;
	miss = 0;
	replace = 0;
	load_hit = 0;
	store_hit = 0;
	load_miss = 0;
	store_miss = 0;
	op_hit = 0;
	op_miss = 0;
	
	std::vector<cache_base> simulation_vector(index_size);
	for(int i = 0; i < index_size; i++){
		simulation_vector[i].lru_policy = lru_base;
		simulation_vector[i].block = std::vector<cache_block>(cache_ways);
		for(int w = 0; w < cache_ways; w++){
			simulation_vector[i].block[w].valid = false;
			simulation_vector[i].block[w].tag = EMPTY_KEY;
		}
	}
	std::cout << "Simulation started..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	
	
	int trace_size = tracefile.size();
	int traceTag = 0;
	int traceIndex = 0;
	
	for(int t = 0; t < trace_size; t++){
		traceTag = bitExtract(tracefile[t].address, tag_bits, index_bits + 1);
		traceIndex = bitExtract(tracefile[t].address, index_bits, offset_bits + 1);
		
		if(optimization){
			if(simulation_vector[traceIndex].block[simulation_vector[traceIndex].lru_policy[0]].tag == traceTag)
				op_hit++;
			else op_miss++;
		}
		
		for(int way = 0; way < cache_ways; way++){
			// search in each way
			if(simulation_vector[traceIndex].block[way].tag == traceTag){							// with index search if tag matches
				hit++; //std::cout << "HIT" << std::endl;	// just for debug purposes
				simulation_vector[traceIndex].block[way].valid = true;								// valid bit set to true if not *TO_REVIEW
				simulation_vector[traceIndex].lru_policy = lru(simulation_vector[traceIndex].lru_policy, way);		// if way is used, set it to first palce in array
				
				if(!tracefile[t].lost) load_hit++;
				if(tracefile[t].lost) store_hit++;
				
				break;																		// data already found	
			}else if(way == cache_ways - 1){													// wait to check last way
				miss++; //std::cout << "MISS" << std::endl;	// just for debug purposes
				if(!tracefile[t].lost) load_miss++;
				if(tracefile[t].lost) store_miss++;
				if(simulation_vector[traceIndex].block[simulation_vector[traceIndex].lru_policy[way]].valid == true)
					replace++;
				
				simulation_vector[traceIndex].block[simulation_vector[traceIndex].lru_policy[way]].tag = traceTag;				// replace/use the last way in array
				simulation_vector[traceIndex].block[simulation_vector[traceIndex].lru_policy[way]].valid = true;					// *TO_REVIEW
				simulation_vector[traceIndex].lru_policy = lru(simulation_vector[traceIndex].lru_policy, way);		// way used, way in first place
			}
		}
		
	}
	
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "Simulation terminated..." << std::endl;
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Simulation elapsed time: " << elapsed.count() << " s" << std::endl << std::endl;
	
}

