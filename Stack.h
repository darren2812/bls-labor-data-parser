#pragma once

#include <functional>
#include <string>
#include "Occupation.h"
#include "DynamicArray.h"

// enum to hold recent state
enum class RecentState {
	ADDED,
	REMOVED
};

// struct to hold job and recent state values (added / deleted)
struct JobPair {
	std::unique_ptr<Occupation> job = {};
	int jobIndex = -1;
	int matrixCodeInt = -1;
	RecentState recentState = RecentState::ADDED;
};

// stack code from zyBooks
class JobStack {
private:
	// allocate 5 elements for the array
	int allocationSize = 5;
	int currentLength = 0;
	JobPair* array = nullptr;

public:

	JobStack();
	~JobStack();
	void resize();
	void push(JobPair &&pair);
	const JobPair& peek() const;
	JobPair pop();
	int getCurrentLength() const;

	void forEachJobInStack(const std::function<void(const JobPair &pair)> &fn) const;
};

