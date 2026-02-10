#include "Stack.h"
#include <iostream>
#include <iomanip>

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
	int newSize = allocationSize * 2;
	JobPair* newArray = new JobPair[newSize];
	for (int i = 0; i < allocationSize; i++) {
		newArray[i] = array[i];
	}
	delete[] array;
	array = newArray;
	allocationSize = newSize;
}

// pushes a pair to the stack and resizes when appropriate
void JobStack::push(JobPair triple) {
	if (currentLength >= allocationSize) {
		resize();
	}
	array[currentLength] = triple;
	currentLength++;
}

// pop function returns pair popped for undo functions
// learned exception handling code from ChatGPT, necessary in this case because function cannot return void
JobPair JobStack::pop() {
	if (currentLength <= 0) {
		throw std::out_of_range("No previous changes are recorded.");
	}
	currentLength--;
	return array[currentLength];
}

// peek function returns pair on top of the stack
JobPair JobStack::peek() {
	if (currentLength <= 0) {
		throw std::out_of_range("No previous changes are recorded.");
	}
	return array[currentLength - 1];
}

// printing the stack accepts an argument to tell if user wants to see recently added or deleted jobs
bool JobStack::printEntireStack(int firstColumnLength, std::string dataset) {
	if (currentLength != 0) {
		int secondColumnLength = 8;
		std::string capitalizedState;
		std::cout << "\nRecent Changes Made to " << dataset << "\n" << std::endl;
		std::cout << std::left << std::setw(firstColumnLength) << "Occupation" << "|";
		// 6 is the length of the word 'removed'
		std::cout << std::left << std::setw(secondColumnLength) << "Action" << "|\n";
		// printing heading horizontal divider
		for (int i = 0; i <= firstColumnLength + secondColumnLength; i++) {
			std::cout << '=';
		}
		std::cout << '|' << std::endl;
		// printing the rest of the table
		for (int i = 0; i < currentLength; i++) {
			capitalizedState = array[i].recentState;
			capitalizedState[0] = toupper(array[i].recentState[0]);
			std::cout << std::left << std::setw(firstColumnLength) << array[i].job.getOccupation() << "|"
				<< std::setw(secondColumnLength) << capitalizedState << "|" << std::endl;
		}
		return true;
	}
	else {
		std::cout << "\nThere are no changes to display at the moment." << std::endl;
		return false;
	}
}

// search function takes in a string to compare with job field in the pair
bool JobStack::searchStack(std::string jobSearched) {
	bool jobFound = false;
	std::cout << std::endl;

	for (int i = 0; i < currentLength; i++) {
		// search for jobs that do not start with the word being searched
		jobSearched[0] = tolower(jobSearched[0]);
		// Chat GPT suggested using .find() instead of making own substring search algorithm
		if (array[i].job.getOccupation().find(jobSearched) != std::string::npos) {
			std::cout << "- " << array[i].job.getOccupation() << std::endl;
			jobFound = true;
		}
		// search for jobs that start with the word being searched (capitalized first letter)
		jobSearched[0] = toupper(jobSearched[0]);
		if (array[i].job.getOccupation().find(jobSearched) != std::string::npos) {
			std::cout << "- " << array[i].job.getOccupation() << std::endl;
			jobFound = true;
		}
	}

	// final return value
	if (jobFound) {
		std::cout << "\nThese jobs were recently added or deleted." << std::endl;
		return true;
	}
	else {
		std::cout << "We could not find any jobs recently added or deleted resembling those letters." << std::endl;
		return false;
	}
}

// getter to check length
int JobStack::getCurrentLength() {
	return currentLength;
}
