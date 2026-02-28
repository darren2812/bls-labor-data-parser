#pragma once

#include <functional>
#include <string>
#include "Occupation.h"
#include "DynamicArray.h"

// struct to hold job and recent state values (added / deleted)
struct JobPair {
	Occupation job;
	std::string recentState;
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
	void push(const JobPair &pair);
	JobPair peek() const;
	JobPair pop();
	bool searchStack(const std::string &jobToSearch, DynamicArray<const Occupation *> &searchedJobsArray);
	int getCurrentLength();

	void forEachJobInStack(const std::function<void(const JobPair &pair)> &fn) const;
};

