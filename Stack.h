#pragma once

#include "Occupation.h"
#include <string>

// struct to hold job and recent state values (added / deleted)
struct JobPair {
	Occupation job;
	std::string recentState;
	// node order set to -1 if not defined (only required for linked list)
	int jobIndex = -1;
};

// stack code from zyBooks
class JobStack {
private:
	// allocate 5 elements for the array
	int allocationSize = 5;
	int currentLength = 0;
	JobPair* array;
public:
	JobStack();
	~JobStack();
	void resize();
	void push(JobPair pair);
	JobPair peek();
	JobPair pop();
	bool searchStack(std::string jobSearched);
	bool printEntireStack(int firstColumnLength, std::string dataset);
	int getCurrentLength();
};

