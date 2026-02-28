#pragma once
#include <functional>
#include "DynamicArray.h"
#include "HashTable.h"
#include "LinkedList.h"

class JobDatabase {
private:
    DynamicArray<std::unique_ptr<Occupation>> allJobsArray;
    HashTable allJobsHashTable;

public:
    JobDatabase() = default;

    JobDatabase(int capacity);

    ~JobDatabase() = default;

    int getSize() const;

    const Occupation *at(int i) const;
    Occupation *at(int i);

    JobDatabase &operator=(const JobDatabase &other) = delete;
    JobDatabase &operator=(JobDatabase &&other) noexcept = default;

    void readInputFile(std::fstream& rawData, const std::string* headings, int* columnLengths);
    void readListFile(std::fstream &listData, SinglyLinkedList list, const HashTable &hashTable);
    void rewriteInputFile(std::fstream& modifiedData);
    void rewriteListFile(std::fstream &listData, const SinglyLinkedList &list);

    bool generateUniqueKey(const std::string &codePrefix, std::string &uniqueCode) const;

    bool searchArrayByJob(DynamicArray<const Occupation *> &searchedJobsArray, const std::string &jobToSearch) const;

    bool searchArrayByWage(DynamicArray<const Occupation *> &searchedJobsArray, int lowerLimit,
                      int upperLimit) const;
    Occupation* searchJobByCode(const int &matrixCodeInt) const;

    const Occupation* findCategory(const std::string &codePrefix) const;

    Occupation* addNewJobToDatabase(const OccupationRow& r);

    bool removeJobFromDatabase(const Occupation *jobPointer);

    // iterators for printing
    void forEachJobInMainArray(const std::function<void(const Occupation *job)> &fn) const;
    void forEachEntryInHashTable(const std::function<void(const Occupation *job)> &fn) const;
    void forEachCategory(const std::function<void(const Occupation *jobCategory)> &fn) const;
    void forEachJobInCategory(const std::string &prefix, const std::function<void(const Occupation *job)> &fn) const;
};
