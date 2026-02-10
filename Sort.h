// Sort.h
// Darren Daniel

#pragma once
#include "Occupation.h"

// getters to act as different arguments when being passed to the mergesort function
std::string getThisOccupation(const Occupation& job);
float getThisWage(const Occupation& job);
int getThisEducationScore(const Occupation& job);
int getThisWorkExperienceScore(const Occupation& job);

// sorting algorithms from zyBooks
// ascending bool for function reusability inspired from Noah's code
// helper function to copy array, initialize indices, and call merge sort
// ChatGPT helped debugged an issue with the pointer by suggesting to pass it by reference

// templated functions and function pointers learned from Google AI and Cherno
// https://www.youtube.com/watch?v=p4sDgQ-jao4
template <typename T>
void sortJob(Occupation* allJobs, Occupation*& sortedJobs, int jobNumber, bool ascending, T(*function)(const Occupation& job)) {
	// assigning the sortedJobs pointer to a dynamic array
	sortedJobs = new Occupation[jobNumber];
	for (int i = 0; i < jobNumber; i++) {
		sortedJobs[i] = allJobs[i];
	}
	int lowIndex = 0;
	int highIndex = jobNumber - 1;
	mergeSortJob(sortedJobs, lowIndex, highIndex, ascending, function);
}
template <typename T>
void mergeSortJob(Occupation*& sortedJobs, int lowIndex, int highIndex, bool ascending, T(*function)(const Occupation& job)) {
	if (lowIndex >= highIndex) {
		return;
	}
	int midIndex = (lowIndex + highIndex) / 2;
	mergeSortJob(sortedJobs, lowIndex, midIndex, ascending, function);
	mergeSortJob(sortedJobs, midIndex + 1, highIndex, ascending, function);
	mergeJob(sortedJobs, lowIndex, midIndex, highIndex, ascending, function);
}
template <typename T>
void mergeJob(Occupation*& sortedJobs, int lowIndex, int midIndex, int highIndex, bool ascending, T(*function)(const Occupation& job)) {
	int leftPos = lowIndex;
	int rightPos = midIndex + 1;
	int mergePos = 0;
	int mergedSize = highIndex - lowIndex + 1;
	Occupation* tempArray = new Occupation[mergedSize];

	while (leftPos <= midIndex && rightPos <= highIndex) {
		// if ascending, compare and take smaller or equal element
		// if not ascending, compare and take larger element
		if ((function(sortedJobs[leftPos]) > function(sortedJobs[rightPos]) && ascending)
			|| (function(sortedJobs[leftPos]) <= function(sortedJobs[rightPos]) && !ascending)) {
			tempArray[mergePos] = sortedJobs[rightPos];
			rightPos++;
		}
		else {
			tempArray[mergePos] = sortedJobs[leftPos];
			leftPos++;
		}
		mergePos++;
	}

	while (leftPos <= midIndex) {
		tempArray[mergePos] = sortedJobs[leftPos];
		mergePos++;
		leftPos++;
	}

	while (rightPos <= highIndex) {
		tempArray[mergePos] = sortedJobs[rightPos];
		mergePos++;
		rightPos++;
	}

	for (int i = 0; i < mergedSize; i++) {
		sortedJobs[lowIndex + i] = tempArray[i];
	}

	delete[] tempArray;
	tempArray = nullptr;
}

// helper function to ask for user sorting preferences
void sortingDialogue(Occupation*& allJobs, Occupation*& sortedJobs, int jobNumber, const std::string* headings, int* columnLengths);