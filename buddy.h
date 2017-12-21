#pragma once
#include<utility>
using namespace std;
class buddy
{
private:
	static int MEMORY_SIZE ;
	pair<int, int> tree[5010];
	pair<int,int>arr[5010];
	int nearstlog[2010];
public:
	buddy();
	// for building the tree
	/*
	l--> start of the memory
	r--> end of the memory   in our case l=1,r=1024
	idx--> send value 1 to it
	val--> memory size  in our case 1024
	*/
	void build(int l, int r, int idx, int val=MEMORY_SIZE); 
	// for assigning a memory location to a process
	/*
	  l,r,idx same as build
	  size--> size of the process
	*/
	pair<int, int>getLocationInMemory(int l, int r, int idx, int size);

	//size of a process
	pair<int, int>allocate(int size);

	// remove a process from memory
	//l,r,idx same as build
	// x,y process section in memory
	void removeFromMemory(int l, int r, int idx, int x, int y, int val=MEMORY_SIZE);

	void buildlog(); //de b7sb lel arkam mn awl 1 le 1024 log2 leha b kam e7tgtha fel algorithm
};
