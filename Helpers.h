// Helpers.h
// Darren Daniel

#pragma once

#include <string>
#include "LinkedList.h"
#include "Occupation.h"
#include "HashTable.h"

// input handler for missing values
float toFloat(std::string& s);
// input handler to standardize input case
void capitalizeFirst(std::string& input);
void lowerString(std::string& input);
// helper function to handle all search functions
bool searchFunction(SinglyLinkedList* list, std::string dataStructure, std::string searchType, Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter,
	const std::string* headings, int* columnLengths, int& searchRows);
// function to find max value in an array
float findMax(Occupation** jobArray, int numberOfJobs, float(*function)(Occupation*));
// function to print out bar chart to the console
void printBarChart(Occupation** comparedJobs, float maxValue, float maxChartLength, int jobsToCompare, float(*function)(Occupation*));