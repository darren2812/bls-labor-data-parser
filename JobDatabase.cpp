#include "JobDatabase.h"

JobDatabase::JobDatabase(const int capacity) :
    allJobsArray(capacity),
    allJobsHashTable(2 * capacity){};

bool JobDatabase::generateUniqueKey(const int& codePrefix, std::string &uniqueCode) const {
    int suffix = 1;
    int codeToReturn = codePrefix * 10000 + suffix;

    // increments suffix if key is not unique
    while (allJobsHashTable.getJobPointer(codeToReturn) != nullptr) {
        if (codeToReturn / 10000 == codePrefix) {
            suffix++;
            codeToReturn = codePrefix * 10000 + suffix;
        }
        else {
            return false;
        }
    }

    uniqueCode = std::to_string(codeToReturn);
    return true;
}

bool JobDatabase::searchByJob(DynamicArray<Occupation*> &searchedJobsArray, const std::string &jobToSearch) {
    searchedJobsArray = DynamicArray<Occupation*>(allJobsArray.getCapacity() / 10);
    searchedJobsArray.searchByJob(allJobsArray, jobToSearch);
    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}

bool JobDatabase::categoryExists(int codePrefix) const {

    const int arraySize = allJobsArray.getCurrentSize();

    for (int i = 0; i < arraySize; i++) {
        if (allJobsArray[i].getMatrixCodeInt() % 10000 == codePrefix) {
            return true;
        }
    }

   return false;
}

void JobDatabase::forEachCategory(const std::function<void(const Occupation& jobCategory)> &fn) const {
    int arraySize = allJobsArray.getCurrentSize();

    for (int i = 0; i < arraySize; i++) {
        if (allJobsArray[i].getMatrixCodeInt() % 10000 == 0) {
           fn(allJobsArray[i]);
        }
    }
}

void JobDatabase::addJobToDatabase(const Occupation &jobToAdd, const std::string &uniqueCode) {
    allJobsHashTable.insertJob(allJobsArray.addJobToArray(jobToAdd));
}




