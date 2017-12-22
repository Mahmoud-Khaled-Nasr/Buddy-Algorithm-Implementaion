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
	return (arrival_time <= time);
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

int process::run(int quanta, int time) {
	//the + 1 is to keep the output the same as the example
	ofstream out;
	out.open(LOG_FILE_NAME, fstream::app);
	out << "Executing process " << id << "\t: started at " << time << ", \t";
	if (run_time > quanta) {
		run_time -= quanta;
		out << "stopped at " << time + quanta << ", " << run_time << " remaining, memory startes at "
			<< location.first << " and ends at " << location.second << endl;
		return quanta ;
	} else {
		int temp = run_time;
		run_time = 0;
		out << "finished at " << time + temp << " ,memory startes at "
			<< location.first << " and ends at " << location.second << endl;
		return temp ;
	}
	out.close();
}

bool process::is_finished() {
	if (run_time == 0) {
		memory->removeFromMemory(0, 1023, 1, location.first, location.second);
		return true;
	} else {
		return false;
	}

}


