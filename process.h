#pragma once
#include<string>
#include<fstream>
#include "buddy.h"

using namespace std;

#define MAX_NUMBER_OF_ALLOCATION_TRIES 5
#define LOG_FILE_NAME "log.txt"

class process {
private:
	int run_time, arrival_time, mem_size;
	//number of times the process didn't found space in memory
	int no_space_in_mem;
	//memory location
	pair<int, int>location;
	//memory
	buddy*memory;
public:
	string id;
	process(string id, int run_time, int arrival_time, int mem_size, buddy* memory);
	bool should_hlt();
	bool is_arrival_time(int time);
	bool can_allocate_mem();
	bool is_allocated_in_mem();
	int run(int quanta, int time);
	bool is_finished();
	void log_hlting(int time);
};

