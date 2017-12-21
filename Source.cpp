#include<iostream>
#include<fstream>
#include<queue>
#include"process.h"
#include"buddy.h"

using namespace std;

const string INPUT_FILE_NAME = "input.txt";

queue<process> read_input_file(int &quanta, int &switch_time, string file_name, buddy* memory) {
	ifstream in;
	in.open(file_name);
	if (! in.is_open()) {
		cout << "can't read the input file\n";
		exit(1);
	}
	string temp;
	in >> temp >> quanta >> temp >> switch_time;
	getline(in, temp, '\n');
	getline(in, temp, '\n');
	string id;
	int run_time, arrival_time, mem_size;
	queue<process> input_processes;
	while (! in.eof()) {
		in >> id >> run_time >> arrival_time >> mem_size;
		input_processes.push(process(id, run_time, arrival_time, mem_size, memory));
	}
	return input_processes;

}

void add_new_processes_to_ready_queue(int time, queue<process> &all_process, queue<process>& ready_queue) {
	while (! all_process.empty()) {
		if (! all_process.front().is_arrival_time(time)) {
			return;
		} else if ( all_process.front().can_allocate_mem()) {
			ready_queue.push(all_process.front());
			all_process.pop();
		} else if (all_process.front().should_hlt () ) {
			//hlting the process
			all_process.pop();
		} else {
			all_process.push(all_process.front());
			all_process.pop();
		}
	}



}


int main() {
	int quanta, switch_time;
	buddy* memory = new buddy();
	queue<process>all_process = read_input_file(quanta, switch_time, INPUT_FILE_NAME, memory);
	queue<process>ready_queue;
	int counter = 0;
	while (!all_process.empty() || !ready_queue.empty()) {
		add_new_processes_to_ready_queue(counter, all_process, ready_queue);
		if (! ready_queue.empty()) {
			if (ready_queue.front().id != ready_queue.back().id) {
				counter += switch_time;
			}
			counter += ready_queue.front().run(quanta);
			if (ready_queue.front().is_finished()) {
				//the process is finished
				//log here
				ready_queue.pop();
			} else {
				//log here for pause
				//put the process at the end of queue
				ready_queue.push(ready_queue.front());
				ready_queue.pop();
			}
		} else {
			counter++;
		}
	}

	delete memory;
}