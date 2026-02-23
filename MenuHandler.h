
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "Helpers.h"
#include "JobDatabase.h"
#include "LinkedList.h"
#include "Sort.h"
#include "Stack.h"

class MenuHandler {
private:

    // integer to store the number of table headings
	static constexpr int NUM_OF_HEADINGS = 16;
	// float to store the maximum length of a bar chart when comparing jobs
	float MAX_BAR_CHART_LENGTH = 100;
	// headings in indexes 1 and 15 are null to skip columns 2 and 16 in data file
	const std::string headings[NUM_OF_HEADINGS] = { "Occupation", "SOC Matrix Code", "Type", "Employment 2023", "Employment 2033",
						   "% Distribution 2023", "% Distribution 2033", "Change 2023-33",
						   "% Change 2023-33", "% Self Employed", "Annual Openings 2023-33",
						   "Median Wage 2024", "Typical Education Needed", "Related Work Experience",
						   "Typical On-the-Job Training", "" };

	// string to take in user input
	std::string userInput = "-";
	std::string jobInput;
	std::string firstHalfKey;
	std::string secondHalfKey;

	// file stream for data
	std::ifstream rawData;
	std::ifstream listData;

	// DATA STRUCTURES
	JobDatabase allJobsDatabase;
	// declaring linked list pointer suggested by chatGPT
	SinglyLinkedList* list = new SinglyLinkedList;
	// creating stacks to track recent changes
	JobStack recentChangesDatabase;
	JobStack recentChangesList;

	// jobCounter counter in main to store the jobCounter of the last job
	int jobCounter = 0;
	// searchRows represents the number of entries successfully searched
	int searchRows = 0;
	// number of total rows in original text file
	int numberOfRows = 0;
	// max number of jobs analyzed
	int totalJobsCapacity = 0;
	// array to assign different lengths for each column and setting everything to 0
	int columnLengths[NUM_OF_HEADINGS] = {};
	// int to store matrix code when searching
	int searchCode;

	// occupation pointer to store occupation being searched
	Occupation* jobSearchedPtr = nullptr;
	// occupation object to store occupation being modified
	Occupation jobModified;
	// Job being undone
	Occupation undoneJob;
	JobPair undoneJobPair;

	// bool to determine whether any searches were found
	bool searchOutcome = false;
	// bool to determine whether user has saved or not
	bool savedDatabase = false;
	bool savedList = false;

	// VARIABLES FOR COMPARISON
	// int to store how many jobs to compare
	short jobsToCompare = 0;
	// int to act as a counter when selecting jobs
	short selectionCounter = 0;
	// float to see the largest number in the comparison
	float maxValue = 0;
	// float to keep track of the value of each hashtag
	float valueOfHashTag = 0;
	// int to keep track of the number of hashtags to print to the console
	int numberOfHashTags = 0;
	// lambda to pass into functions when printing bar charts
	float getterLambda;

public:
	MenuHandler();
	void allocateDataStructures();

	// table related functions
	void printTableHeadings(std::ofstream& output, const std::string* headings, int* columnLengths) const;
	void printTableEntry(std::ofstream& output, int* columnLengths, Occupation& currentJob) const;
	void formatAndPrintArray(const DynamicArray &array, std::string *headings, int *columnLengths) const;
	// method to display all contents of the hash table
	void formatAndPrintHashTable(const HashTable &table, std::string *headings, int *columnLengths) const;

	// job adding prompts
	Occupation promptJobAttributes(std::string jobTitle);
	std::string promptNonNegativeOrDash();
	int promptMatrixCodePrefix();
	void handleAddJob();
};