#include "process.h"


process::process(string id,int run_time, int arrival_time, int mem_size, buddy* memory) {
	this->id = id;
	this->run_time = run_time;
	this->arrival_time = arrival_time;
	this->mem_size = mem_size;
	this->memory = memory;
	no_space_in_mem = 0;
	location = pair<int, int>(-1, -1);
}

bool process::should_hlt() {
	return (no_space_in_mem > MAX_NUMBER_OF_ALLOCATION_TRIES);
}

bool process::is_arrival_time(int time) {
	return (arrival_time == time);
}

bool process::can_allocate_mem() {
	location = memory->allocate(mem_size);
	if (location.first != -1 && location.second != -1) {
		return true;
	} else {
		no_space_in_mem++;
		return false;
	}
}

bool process::is_allocated_in_mem() {
	return !(location.first == -1 && location.second == -1);
}

int process::run(int quanta) {
	if (run_time > quanta) {
		run_time -= quanta;
		return quanta;
	} else {
		int temp = run_time;
		run_time = 0;
		return temp;
	}
}

bool process::is_finished() {
	if (run_time == 0) {
		memory->removeFromMemory(1, 1024, 1, location.first, location.second);
		return true;
	} else {
		return false;
	}

}


