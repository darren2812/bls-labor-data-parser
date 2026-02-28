// Sort.h
// Darren Daniel

#pragma once
#include "Occupation.h"
#include "DynamicArray.h"

template <typename Lambda>
void sortJob(DynamicArray<const Occupation *> &sortedJobs, bool ascending, Lambda &&getData) {

	int lowIndex = 0;
	int highIndex = sortedJobs.getCurrentSize() - 1;

	// based on ascending / descending, function compares and returns the right element
	auto compare = [&](const Occupation *a, const Occupation *b) {
		if (ascending) {
			return getData(a) <= getData(b);
		}
		return getData(a) > getData(b);
	};

	mergeSortJob(sortedJobs, lowIndex, highIndex, compare);
}
template <typename Lambda>
void mergeSortJob(DynamicArray<const Occupation *> &sortedJobs, int lowIndex, int highIndex, Lambda compare) {
	if (lowIndex >= highIndex) {
		return;
	}
	int midIndex = (lowIndex + highIndex) / 2;
	mergeSortJob(sortedJobs, lowIndex, midIndex, compare);
	mergeSortJob(sortedJobs, midIndex + 1, highIndex, compare);
	mergeJob(sortedJobs, lowIndex, midIndex, highIndex, compare);
}

template <typename Lambda>
void mergeJob(DynamicArray<const Occupation *> &sortedJobs, int lowIndex, int midIndex, int highIndex, Lambda compare) {
	int leftPos = lowIndex;
	int rightPos = midIndex + 1;
	int mergePos = 0;
	int mergedSize = highIndex - lowIndex + 1;
	auto tempArray = new const Occupation* [mergedSize];

	while (leftPos <= midIndex && rightPos <= highIndex) {
		// if ascending, compare and take smaller or equal element
		// if not ascending, compare and take larger element
		if (compare(sortedJobs[leftPos], sortedJobs[rightPos])) {
			tempArray[mergePos] = sortedJobs[leftPos];
			leftPos++;
		}
		else {
			tempArray[mergePos] = sortedJobs[rightPos];
			rightPos++;
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