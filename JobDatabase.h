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

    JobDatabase &operator=(const JobDatabase &other) = delete;
    JobDatabase &operator=(JobDatabase &&other) noexcept = default;

    void readInputFile(std::fstream& rawData, const std::string* headings, int* columnLengths);
    void readListFile(std::fstream &listData, SinglyLinkedList list, const HashTable &hashTable);
    void rewriteInputFile(std::fstream& modifiedData);
    void rewriteListFile(std::fstream &listData, const SinglyLinkedList &list);

    bool generateUniqueKey(const std::string &codePrefix, std::string &uniqueCode) const;

    bool searchArrayByJob(DynamicArray<Occupation *> &searchedJobsArray, const std::string &jobToSearch) const;

    bool searchArrayByWage(DynamicArray<Occupation *> &searchedJobsArray, const float &lowerLimit,
                      const float &upperLimit) const;
    Occupation* searchJobByCode(const int &matrixCodeInt) const;

    Occupation* findCategory(const std::string &codePrefix) const;

    // for each category iterator for printing
    void forEachJobInMainArray(const std::function<void(const Occupation &job)> &fn) const;
    void forEachEntryInHashTable(const std::function<void(const Occupation &job)> &fn) const;
    void forEachCategory(const std::function<void(const Occupation &jobCategory)> &fn) const;
    void forEachJobInCategory(const std::string &prefix, const std::function<void(const Occupation &job)> &fn) const;

    Occupation* addNewJobToDatabase(const OccupationRow& r);
    void removeJobFromDatabase(const Occupation* jobPointer);
};
