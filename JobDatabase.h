#pragma once
#include <functional>
#include "DynamicArray.h"
#include "HashTable.h"

class JobDatabase {
private:
    DynamicArray allJobsArray;
    DynamicArray searchedJobsArray;
    DynamicArray allJobsArraySorted;
    HashTable allJobsHashTable;
public:
    JobDatabase() = default;
    JobDatabase(int capacity);
    ~JobDatabase() = default;

    JobDatabase& operator=(const JobDatabase& other) = default;

    bool generateUniqueKey(const int &codePrefix, std::string &uniqueCode) const;
    bool searchByJob(const std::string& jobToSearch);
    bool categoryExists(const DynamicArray& array, int codePrefix) const;

    // for each job iterators for printing
    void forEachSearchedJob(std::function<void(Occupation const &currentJob)> fn) const;
    void forEachCategory(const std::function<void(const Occupation& jobCategory)> &fn) const;

    void addJobToDatabase(const Occupation &jobToAdd, const std::string &uniqueCode);

    // FINISH ADD FUNCTION
};