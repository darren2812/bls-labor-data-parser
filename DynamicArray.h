#pragma once

#include <string>
#include "Occupation.h"

class DynamicArray {
private:
    Occupation* data;
    int capacity;
    int currentSize = 0;

public:
    DynamicArray();
    DynamicArray(int capacity);

    // overloading the equal operator to copy
    DynamicArray& operator=(const DynamicArray& other);
    // overloading [] operator to access job at that index
    Occupation& operator[](int index) const;

    ~DynamicArray();

    int getCurrentSize() const;
    int getCapacity() const;

    void increaseCapacity();

    // function to read entries from text file
    void readEntries(std::ifstream& rawData, const std::string* headings, int* columnLengths);

    // function to search by job title
    int searchByJob(const DynamicArray& allJobs, const std::string& jobSearched);

    // function to search by wage
    int searchByWage(const DynamicArray& allJobs, const int& jobCounter, const float& lowerLimit,
        const float& upperLimit);

    Occupation addJobToArray(const Occupation &jobToAdd);

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