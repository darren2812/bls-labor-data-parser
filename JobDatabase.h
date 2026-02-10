// JobDatabase.h
// Darren Daniel

#pragma once
#include <string>
#include "Occupation.h"
#include "HashTable.h"

// function to read entries
void readEntries(std::ifstream& rawData, Occupation* allJobs, const std::string* headings, int* columnLengths, int& jobNumber);

// function to view entries
void viewEntries(Occupation* allJobs, const std::string* headings, int* columnLengths, const int& jobNumber);

// function to search by job title
int searchByJob(Occupation* searchedJobs, Occupation* allJobs, const int& jobNumber, std::string jobSearched);

// function to search by wage
int searchByWage(Occupation* searchedJobs, Occupation* allJobs, const int& jobNumber, const float& lowerLimit,
	const float& upperLimit);

// function to add entries
// Gemini helped fixed a bug because the pointer in the function is changed when creating an expanded array but not in main()
// Hence, allJobs has to be passed by reference
Occupation addEntry(Occupation*& allJobs, int& jobNumber, int& totalJobsCapacity, std::string& jobAdded, int* columnLengths, std::string key);

// function to readd a deleted job
void addEntryAgain(Occupation jobAdded, Occupation*& allJobs, int& jobNumber);

// function to modify job input file
void rewriteJobFile(Occupation* allJobs, const int& jobNumber);

// function to remove an entry
Occupation removeEntry(Occupation*& allJobs, int& jobNumber, int& totalJobsCapacity, int indexRemoved);

// function to read list from file
void importList(std::ifstream& listData, Occupation* allJobs, SinglyLinkedList* list, int jobNumber, HashTable& hashTable);

// function to modify list input file
void rewriteListFile(SinglyLinkedList* list);
