#include "Stack.h"
#include <iostream>
#include <iomanip>

#include "Helpers.h"

// constructor creates a new dynamic array of job pairs
JobStack::JobStack() {
	array = new JobPair[allocationSize];
}

// destructor frees up memory
JobStack::~JobStack() {
	delete[] array;
	array = nullptr;
}

// dynamically resizes the jobstack
void JobStack::resize() {
	const int newSize = allocationSize == 0 ? 1 : allocationSize * 2;
	auto newArray = new JobPair[newSize];

	for (int i = 0; i < currentLength; i++) {
		newArray[i] = std::move(array[i]);
	}

	delete[] array;
	array = newArray;
	allocationSize = newSize;
}

// pushes a pair to the stack and resizes when appropriate
void JobStack::push(const JobPair &pair) {
	if (currentLength >= allocationSize) {
		resize();
	}
	array[currentLength] = pair;
	currentLength++;
}

JobPair JobStack::pop() {
	if (currentLength <= 0) {
		throw std::out_of_range("No previous changes are recorded.");
	}
	currentLength--;
	return array[currentLength];
}

JobPair JobStack::peek() const {
	if (currentLength <= 0) {
		throw std::out_of_range("No previous changes are recorded.");
	}
	return array[currentLength - 1];
}

// search function takes in a string to compare with job field in the pair
bool JobStack::searchStack(const std::string &jobToSearch, DynamicArray<const Occupation *> &searchedJobsArray) {

	searchedJobsArray = DynamicArray<const Occupation*>(currentLength);

	// linear search for substrings
	for (int i = 0; i < currentLength; i++) {
		std::string query = jobToSearch;
		std::string currentEntry = array[i].job.getOccupation();
		lowerString(query);
		lowerString(currentEntry);

		if (currentEntry.find(query) != std::string::npos) {
			searchedJobsArray.append(&array[i].job);
		}
	}

	if (searchedJobsArray.getCurrentSize() > 0) {
		return true;
	}
	return false;
}

// getter to check length
int JobStack::getCurrentLength() {
	return currentLength;
}

void JobStack::forEachJobInStack(const std::function<void(const JobPair &pair)> &fn) const {
	for (int i = 0; i < currentLength; i++) {
		fn(array[i]);
	}
}
