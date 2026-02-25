#pragma once
#include <functional>
#include "DynamicArray.h"
#include "HashTable.h"

class JobDatabase {
private:
    DynamicArray<std::unique_ptr<Occupation>> allJobsArray;
    HashTable allJobsHashTable;
public:
    JobDatabase() = default;
    JobDatabase(int capacity);
    ~JobDatabase() = default;

    JobDatabase& operator=(const JobDatabase& other) = default;

    bool generateUniqueKey(const int &codePrefix, std::string &uniqueCode) const;
    bool searchByJob(DynamicArray<Occupation*> &searchedJobsArray, const std::string& jobToSearch);
    bool categoryExists(int codePrefix) const;

    // for each category iterator for printing
    void forEachCategory(const std::function<void(const Occupation& jobCategory)> &fn) const;

    void addJobToDatabase(const Occupation &jobToAdd, const std::string &uniqueCode);

};