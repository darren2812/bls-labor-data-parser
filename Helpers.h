// Helpers.h
// Darren Daniel

#pragma once

#include <string>
#include <fstream>
#include "LinkedList.h"
#include "Occupation.h"
#include "JobDatabase.h"
#include "HashTable.h"

// input handler for missing values
float toFloat(std::string& s);
// input handler to standardize input case
void lowerString(std::string& input);
// global boolean to determine whether catch is called or not
extern bool g_catch;
// function to print table headings
void printTableHeadings(std::ofstream& output, const std::string* headings, int* columnLengths);
// function to print table entry
void printTableEntry(std::ofstream& output, int* columnLengths, Occupation& currentJob);
// function to select a specific index from the database
Occupation* selectSpecficIndex(Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter, std::string& userInput, std::string command);
// function to handle menu selection for reusability
char menuHandling(char firstLetter, char lastLetter);
// function to handle yes/no selection for reusability
char yesOrNoMenu();
// helper function to handle all search functions
bool searchFunction(SinglyLinkedList* list, std::string dataStructure, std::string searchType, Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter,
	const std::string* headings, int* columnLengths, int& searchRows);
// helper function to search for a particular job in the hash table
Occupation* buildKeyAndSearch(Occupation* allJobs, int jobCounter, HashTable& hashTable);
// function to find max value in an array
float findMax(Occupation**& jobArray, int numberOfJobs, float(*function)(Occupation*&));
// function to print out bar chart to the console
void printBarChart(Occupation**& comparedJobs, float maxValue, float maxChartLength, int jobsToCompare, float(*function)(Occupation*&));