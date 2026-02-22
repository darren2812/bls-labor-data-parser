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
            std::cout << "\nThis category already contains the maximum number of jobs."
            << " Please choose a different category." << std::endl;
            return false;
        }
    }

    uniqueCode = std::to_string(codeToReturn);
    return true;
}

bool JobDatabase::searchByJob(const std::string &jobToSearch) {
    searchedJobsArray = DynamicArray(allJobsArray.getCapacity() / 10);
    searchedJobsArray.searchByJob(allJobsArray, jobToSearch);
    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}




