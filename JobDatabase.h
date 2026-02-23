#pragma once
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

    void viewSearchedJobs() const;
};