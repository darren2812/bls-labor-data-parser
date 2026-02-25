#pragma once

#include <string>
#include "Occupation.h"

template <typename T>
class DynamicArray {
private:
    T data = nullptr;
    int capacity = 0;
    int currentSize = 0;

public:
    DynamicArray() = default;
    ~DynamicArray();
    DynamicArray(int capacity);

    // overloading [] operator to access job at that index
    Occupation& operator[](int index) const;

    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    DynamicArray(DynamicArray&&) noexcept;
    DynamicArray& operator=(DynamicArray&&) noexcept;

    int getCurrentSize() const;
    int getCapacity() const;

    void increaseCapacity();

    // function to read entries from text file
    void readInputFile(std::fstream& rawData, const std::string* headings, int* columnLengths);

    // function to search by job title
    int searchByJob(const DynamicArray& allJobs, const std::string& jobSearched);

    // function to search by wage
    int searchByWage(const DynamicArray& allJobs, const int& jobCounter, const float& lowerLimit,
        const float& upperLimit);

    Occupation addJobToArray(const Occupation &jobToAdd);

    // function to readd a deleted job
    void addEntryAgain(const Occupation &jobAdded);

    // function to modify job input file
    void rewriteInputFile(std::fstream& modifiedData);

    // function to remove an entry
    Occupation removeEntry(int indexRemoved);

    // function to read list from file
    void importList(std::ifstream& listData, const DynamicArray& allJobs, SinglyLinkedList* list, int jobCounter, HashTable& hashTable);

    // function to modify list input file
    void rewriteListFile(SinglyLinkedList* list);
};