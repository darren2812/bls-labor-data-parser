#pragma once
#include <functional>
#include "DynamicArray.h"
#include "HashTable.h"
#include "LinkedList.h"

class JobDatabase {
private:
    DynamicArray<std::unique_ptr<Occupation> > allJobsArray;
    HashTable allJobsHashTable;

public:
    JobDatabase() = default;

    JobDatabase(int capacity);

    ~JobDatabase() = default;

    JobDatabase &operator=(const JobDatabase &other) = delete;
    JobDatabase &operator=(JobDatabase &&other) noexcept = default;

    void readInputFile(std::fstream& rawData, const std::string* headings, int* columnLengths);
    void importList(std::fstream &listData, SinglyLinkedList *list, const HashTable &hashTable);
    void rewriteListFile(std::fstream &listData, SinglyLinkedList* list);

    bool generateUniqueKey(const int &codePrefix, std::string &uniqueCode) const;

    bool searchArrayByJob(DynamicArray<Occupation *> &searchedJobsArray, const std::string &jobToSearch) const;

    bool searchArrayByWage(DynamicArray<Occupation *> &searchedJobsArray, const float &lowerLimit,
                      const float &upperLimit) const;

    bool categoryExists(int codePrefix) const;

    // for each category iterator for printing
    void forEachCategory(const std::function<void(const Occupation &jobCategory)> &fn) const;

    void addJobToDatabase(const Occupation &jobToAdd);
};
