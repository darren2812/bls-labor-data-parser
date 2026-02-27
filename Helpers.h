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
void lowerString(std::string& input);
// global boolean to determine whether catch is called or not
extern bool g_catch;
// function to select a specific index from the database
Occupation* selectSpecificIndex(Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter, std::string& userInput, std::string command);
// helper function to handle all search functions
bool searchFunction(SinglyLinkedList* list, std::string dataStructure, std::string searchType, Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter,
	const std::string* headings, int* columnLengths, int& searchRows);
// function to find max value in an array
float findMax(Occupation**& jobArray, int numberOfJobs, float(*function)(Occupation*&));
// function to print out bar chart to the console
void printBarChart(Occupation**& comparedJobs, float maxValue, float maxChartLength, int jobsToCompare, float(*function)(Occupation*&));