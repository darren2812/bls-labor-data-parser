#include <iostream>
#include <fstream>
#include "JobDatabase.h"
#include "Helpers.h"

JobDatabase::JobDatabase(const size_t capacity) :
    allJobsArray(capacity),
    allJobsHashTable(2 * capacity){};

int JobDatabase::getSize() const {
    return allJobsArray.getCurrentSize();
}

void JobDatabase::readInputFile(std::fstream &rawData, size_t *columnLengths) {
    rawData.clear();
    rawData.seekg(0, std::ios::beg);
    std::string tempString;
    // counters
    int currentSize = 0;
    short columnCount = 0;

    OccupationRow row;

    /*
        Case 0: Occupation
        Case 1: Matrix Code
        Case 2: Occupation type
        Case 3: Employment Current
        Case 4: Employment Future
        Case 5: Distribution Current
        Case 6: Distribution Future
        Case 7: Numeric Change
        Case 8: Percent Change
        Case 9: Percent Self Employed
        Case 10: Job Openings
        Case 11: Wage
        Case 12: Education
        Case 13: Work experience
        Case 14: Training
        Case 15: Set columnCount to -1 so that it becomes 0 again after increment
                 and increment jobCounter to move to the next jobCounter of the dynamic array
        */

    while (std::getline(rawData, tempString)) {
        switch (columnCount) {
            case 0:  row.occupation = tempString; break;
            case 1:  row.matrixCode = tempString; break;
            case 2:  row.occupationType = tempString; break;
            case 3:  row.employmentCurrent = tempString; break;
            case 4:  row.employmentFuture  = tempString; break;
            case 5:  row.distributionCurrent = tempString; break;
            case 6:  row.distributionFuture  = tempString; break;
            case 7:  row.numericChange = tempString; break;
            case 8:  row.percentageChange = tempString; break;
            case 9:  row.percentSelfEmployed = tempString; break;
            case 10: row.jobOpenings = tempString; break;
            case 11: row.wage = tempString; break;
            case 12: row.education = tempString; break;
            case 13: row.workExperience = tempString; break;
            case 14: row.training = tempString; break;
            case 15:
                row.handbookContent = tempString;
                addNewJobToDatabase(row);
                row = {};           // reset to defaults
                columnCount = -1;
                currentSize++;
                break;
            default: break;
        }
        // skips matrix and handbook columns in file and finds the longest std::string from input to adjust for column size
        if (columnCount != 1 && tempString.size() + 1 > columnLengths[columnCount]) {
            columnLengths[columnCount] = tempString.size() + 1;
        }
        // increments columnCount
        columnCount++;
    }
}

void JobDatabase::readListFile(std::fstream &listData, SinglyLinkedList &list) const {
    // setting the cursor position back to the start
    listData.clear();
    listData.seekg(0, std::ios::beg);
    std::string initialString;
    std::string tempString;
    // loop while file does not end
    while (std::getline(listData, tempString)) {
        // try catch block to handle erroneous input
        try {
            initialString = tempString;
            // erases the - character from the matrix code
            auto pos = tempString.find('-');
            if (pos != std::string::npos) {
                tempString.erase(pos, 1);
            }
            // converts the modified code to an int
            int matrixCode = stoi(tempString);
            // searches for that code in the string
            const Occupation* jobPointer = allJobsHashTable.getJobPointer(matrixCode);
            if (jobPointer) {
                list.append(jobPointer);
            } else {
                std::cout << "\nFailed to find occupation with matrix code " << initialString << std::endl;
            }
        } catch (std::exception&) {
            std::cout << "\nSkipping " << initialString << " from list input file." << std::endl;
        }
    }
}

void JobDatabase::rewriteInputFile(const std::string &modifiedDataPath, std::fstream &modifiedData) {

    modifiedData.close();
    modifiedData.open(modifiedDataPath, std::ios::out | std::ios::trunc);

    int arraySize = allJobsArray.getCurrentSize();

    for (int i = 0; i < arraySize; i++) {
        modifiedData << allJobsArray[i]->getOccupation() << std::endl
                << allJobsArray[i]->getMatrixCode() << std::endl
                << allJobsArray[i]->getOccupationType() << std::endl
                << allJobsArray[i]->getEmploymentCurrentString() << std::endl
                << allJobsArray[i]->getEmploymentFutureString() << std::endl
                << allJobsArray[i]->getDistributionCurrentString() << std::endl
                << allJobsArray[i]->getDistributionFutureString() << std::endl
                << allJobsArray[i]->getNumericChangeString() << std::endl
                << allJobsArray[i]->getPercentageChangeString() << std::endl
                << allJobsArray[i]->getPercentSelfEmployedString() << std::endl
                << allJobsArray[i]->getJobOpeningsString() << std::endl
                << allJobsArray[i]->getWageString() << std::endl
                << allJobsArray[i]->getEducation() << std::endl
                << allJobsArray[i]->getWorkExperience() << std::endl
                << allJobsArray[i]->getTraining() << std::endl
                << allJobsArray[i]->getHandbookContent() << std::endl;
    }
}

void JobDatabase::rewriteListFile(const std::string &listDataPath, std::fstream &listData, const SinglyLinkedList &list) {

    listData.close();
    listData.open(listDataPath, std::ios::out | std::ios::trunc);

    if (list.getListSize() > 0) {
        list.forEachJobInList([&](const Occupation *job) {
            listData << job->getMatrixCode() << std::endl;
        });
        return;
    }

    std::cout << "\nThe list is empty." << std::endl;
}

bool JobDatabase::generateUniqueKey(const std::string& codePrefix, std::string &uniqueCode) const {
    int suffix = 1;
    int codeToReturn;

    // increments suffix if key is not unique
    do {
        codeToReturn = std::stoi(codePrefix) * 10000 + suffix;
        if (codeToReturn / 10000 == std::stoi(codePrefix)) {
            suffix++;
        }
        else {
            return false;
        }
    } while (allJobsHashTable.getJobPointer(codeToReturn) != nullptr);

    uniqueCode = std::to_string(codeToReturn);
    uniqueCode.insert(codePrefix.length(), "-");

    return true;
}

bool JobDatabase::searchArrayByJob(DynamicArray<const Occupation*> &searchedJobsArray, const std::string &jobToSearch) const{

    int mainArraySize = allJobsArray.getCurrentSize();
    searchedJobsArray = DynamicArray<const Occupation*>(mainArraySize);

    // linear search for substrings
    for (int i = 0; i < mainArraySize; i++) {
        std::string query = jobToSearch;
        std::string currentEntry = allJobsArray[i]->getOccupation();
        lowerString(query);
        lowerString(currentEntry);

        if (currentEntry.find(query) != std::string::npos) {
            searchedJobsArray.append(allJobsArray[i].get());
        }
    }

    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}

bool JobDatabase::searchArrayByWage(DynamicArray<const Occupation *> &searchedJobsArray, const int lowerLimit, const int upperLimit) const {

    int mainArraySize = allJobsArray.getCurrentSize();
    searchedJobsArray = DynamicArray<const Occupation*>(mainArraySize);

    // linear search to find whether wage sits between the upper and lower bounds inclusive
    for (int i = 0; i < mainArraySize; i++) {
        if (allJobsArray[i]->getWage() >= lowerLimit && allJobsArray[i]->getWage() <= upperLimit) {
            searchedJobsArray.append(allJobsArray[i].get());
        }
    }

    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}

const Occupation *JobDatabase::searchJobByCode(const int &matrixCodeInt) const {
    return allJobsHashTable.getJobPointer(matrixCodeInt);
}

const Occupation* JobDatabase::findCategory(const std::string &codePrefix) const {

    const int arraySize = allJobsArray.getCurrentSize();

    for (int i = 0; i < arraySize; i++) {
        if (allJobsArray[i]->getMatrixCodeInt() / 10000 == stoi(codePrefix)) {
            return allJobsArray[i].get();
        }
    }

   return nullptr;
}

void JobDatabase::forEachJobInMainArray(const std::function<void(const Occupation *job)> &fn) const {
    int arraySize = allJobsArray.getCurrentSize();

    for (int i = 0; i < arraySize; i++) {
        fn(allJobsArray[i].get());
    }
}

void JobDatabase::forEachEntryInHashTable(const std::function<void(const Occupation *job)> &fn) const {
    int hashTableSize = allJobsHashTable.getTableCapacity();

    for (int i = 0; i < hashTableSize; i++) {
        if (!allJobsHashTable[i]->isEmpty()) {
            fn(allJobsHashTable[i]->JobPointer);
        }
    }
}

void JobDatabase::forEachCategory(const std::function<void(const Occupation *jobCategory)> &fn) const {
    int arraySize = allJobsArray.getCurrentSize();

    for (int i = 0; i < arraySize; i++) {
        if (allJobsArray[i]->getMatrixCodeInt() % 10000 == 0) {
           fn(allJobsArray[i].get());
        }
    }
}

void JobDatabase::forEachJobInCategory(const std::string &prefix, const std::function<void(const Occupation *job)> &fn) const {
    int arraySize = allJobsArray.getCurrentSize();
    for (int i = 0; i < arraySize; i++) {
        if (allJobsArray[i]->getMatrixPrefix() == prefix) {
            fn(allJobsArray[i].get());
        }
    }
}

Occupation* JobDatabase::addNewJobToDatabase(const OccupationRow& r) {

    auto uniquePointer = std::make_unique<Occupation>();

    uniquePointer->setOccupation(r.occupation);
    uniquePointer->setMatrixCode(r.matrixCode);
    uniquePointer->setOccupationType(r.occupationType);
    uniquePointer->setEmploymentCurrent(r.employmentCurrent);
    uniquePointer->setEmploymentFuture(r.employmentFuture);
    uniquePointer->setDistributionCurrent(r.distributionCurrent);
    uniquePointer->setDistributionFuture(r.distributionFuture);
    uniquePointer->setNumericChange(r.numericChange);
    uniquePointer->setPercentageChange(r.percentageChange);
    uniquePointer->setPercentSelfEmployed(r.percentSelfEmployed);
    uniquePointer->setJobOpenings(r.jobOpenings);
    uniquePointer->setWage(r.wage);
    uniquePointer->setEducation(r.education);
    uniquePointer->setWorkExperience(r.workExperience);
    uniquePointer->setTraining(r.training);
    uniquePointer->setHandbookContent(r.handbookContent);
    uniquePointer->calculateChanges();

    Occupation* rawPointer = uniquePointer.get();
    allJobsArray.addJobToMainArray(std::move(uniquePointer), allJobsArray.getCurrentSize());
    allJobsHashTable.insertJob(rawPointer);

    return rawPointer;
}

void JobDatabase::restoreJob(std::unique_ptr<Occupation> jobToAdd, int index) {
    Occupation* rawPointer = jobToAdd.get();
    allJobsArray.addJobToMainArray(std::move(jobToAdd), index);
    allJobsHashTable.insertJob(rawPointer);
}

std::unique_ptr<Occupation> JobDatabase::removeJobFromDatabase(const int indexToRemove, const int matrixCodeInt) {
    std::unique_ptr<Occupation> jobToRemove = allJobsArray.removeEntry(indexToRemove);

    if (jobToRemove != nullptr && allJobsHashTable.removeJob(matrixCodeInt)) {
        return jobToRemove;
    }
    return nullptr;
}


