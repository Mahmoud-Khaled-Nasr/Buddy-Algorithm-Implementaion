#include<iostream>
#include<fstream>
#include<queue>
#include"process.h"
#include"buddy.h"

using namespace std;

#define INPUT_FILE_NAME "input.txt"
#define LOG_FILE_NAME "log.txt"


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
	while (! all_process.empty() && all_process.front().is_arrival_time(time)) {
		ready_queue.push(all_process.front());
		all_process.pop();
	}
}


void print_queue(queue<process> q) {
	ofstream out;
	out.open(LOG_FILE_NAME, fstream::app);
	if (!out.is_open()) {
		cout << "The log file can't be opened\n";
		exit(1);
	}
	out << "Queue:";
	while (! q.empty()) {
		out << " " << q.front().id;
		q.pop();
		if (q.size() != 0) {
			out << ",";
		}
	}
	out << endl;
	out.close();
}

int switching(int counter, int switch_time) {
	//the + 1 is to keep the output the same as the example
	ofstream out;
	out.open(LOG_FILE_NAME, fstream::app);
	counter++;
	out << "Process switching\t: started at " << counter;
	counter += switch_time;
	out << ",\tfinished at " << counter << endl;
	out.close();
	//To make the next process starts from the next sec 
	counter++;
	return counter;
}

void open_log_file() {
	//open the log file
	ofstream out;
	out.open(LOG_FILE_NAME);
	if (!out.is_open()) {
		cout << "The log file can't be opened\n";
		exit(1);
	}
}

int main() {
	open_log_file();
	int quanta, switch_time;
	buddy* memory = new buddy();
	queue<process>all_process = read_input_file(quanta, switch_time, INPUT_FILE_NAME, memory);
	queue<process>ready_queue;
	process* current_process_occupies_the_processor = NULL;
	int counter = 0;
	while (!all_process.empty() || !ready_queue.empty()) {
		add_new_processes_to_ready_queue(counter, all_process, ready_queue);
		if (! ready_queue.empty()) {
			if (current_process_occupies_the_processor != NULL ) {
				if (ready_queue.front().id != current_process_occupies_the_processor->id) {
					counter = switching(counter, switch_time);
				}
			}
			//log the queue to the file
			print_queue(ready_queue);
			//check for memory conditions
			if (ready_queue.front().can_allocate_mem()) {
				//run the process
				current_process_occupies_the_processor = &ready_queue.front();
				counter += ready_queue.front().run(quanta, counter);
				if (ready_queue.front().is_finished()) {
					//the process is finished
					ready_queue.pop();
				} else {
					//log here for pause
					// check for new processes to keep the order
					add_new_processes_to_ready_queue(counter, all_process, ready_queue);
					//put the process at the end of queue
					ready_queue.push(ready_queue.front());
					ready_queue.pop();
				}
			} else if (ready_queue.front().should_hlt()) {
				ready_queue.front().log_hlting(counter);
				ready_queue.pop();
			} else {
				ready_queue.push(ready_queue.front());
				ready_queue.pop();
			}
		} else {
			counter++;
		}
	}

	delete memory;
}