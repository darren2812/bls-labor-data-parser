#pragma once

#include <string>
#include "Occupation.h"
#include "HashTable.h"
#include "menuHandler.h"

class DynamicArray {
private:
    Occupation* data;
    int capacity;
    int currentSize = 0;

public:
    DynamicArray(int& capacity);

    void increaseCapacity();

    // function to read entries
    void readEntries(std::ifstream& rawData, const std::string* headings, int* columnLengths, int& jobCounter);

    // function to view entries
    void viewEntries(const std::string* headings, int* columnLengths, const int& jobCounter) const;

    // function to search by job title
    int searchByJob(const DynamicArray& allJobs, const int& jobCounter, std::string& jobSearched);

    // function to search by wage
    int searchByWage(const DynamicArray& allJobs, const int& jobCounter, const float& lowerLimit,
        const float& upperLimit);

    // function to add entries
    // Gemini helped fixed a bug because the pointer in the function is changed when creating an expanded array but not in main()
    // Hence, allJobs has to be passed by reference
    Occupation addEntry(DynamicArray& allJobs, int& jobCounter, int& totalJobsCapacity, std::string& jobAdded, const int* columnLengths, const std::string& key);

    // function to readd a deleted job
    void addEntryAgain(Occupation jobAdded, const DynamicArray& allJobs, int& jobCounter);

    // function to modify job input file
    void rewriteJobFile(const DynamicArray& allJobs, const int& jobCounter);

    // function to remove an entry
    Occupation removeEntry(const DynamicArray& allJobs, int& jobCounter, int& totalJobsCapacity, int indexRemoved);

    // function to read list from file
    void importList(std::ifstream& listData, const DynamicArray& allJobs, SinglyLinkedList* list, int jobCounter, HashTable& hashTable);

    // function to modify list input file
    void rewriteListFile(SinglyLinkedList* list);
};