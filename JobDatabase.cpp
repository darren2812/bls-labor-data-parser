#include <iostream>
#include "JobDatabase.h"
#include "Helpers.h"

JobDatabase::JobDatabase(const int capacity) :
    allJobsArray(capacity),
    allJobsHashTable(2 * capacity){};

void JobDatabase::readInputFile(std::fstream &rawData, const std::string *headings, int *columnLengths) {
    rawData.clear();
    rawData.seekg(0, std::ios::beg);
    std::string tempString;
    // counters
    int currentSize = 0;
    short columnCount = 0;

    Occupation jobToAdd;

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

    // reads through the entire text file line by line
    while (std::getline(rawData, tempString)) {
        switch (columnCount) {
            case 0:
                jobToAdd.setOccupation(tempString);
                break;
            case 1:
                jobToAdd.setMatrixCode(tempString);
                break;
            case 2:
                jobToAdd.setOccupationType(tempString);
                break;
            case 3:
                jobToAdd.setEmploymentCurrent(tempString);
                break;
            case 4:
                jobToAdd.setEmploymentFuture(tempString);
                break;
            case 5:
                jobToAdd.setDistributionCurrent(tempString);
                break;
            case 6:
                jobToAdd.setDistributionFuture(tempString);
                break;
            case 7:
                jobToAdd.setNumericChange(tempString);
                break;
            case 8:
                jobToAdd.setPercentageChange(tempString);
                break;
            case 9:
                jobToAdd.setPercentSelfEmployed(tempString);
                break;
            case 10:
                jobToAdd.setJobOpenings(tempString);
                break;
            case 11:
                jobToAdd.setWage(tempString);
                break;
            case 12:
                jobToAdd.setEducation(tempString);
                break;
            case 13:
                jobToAdd.setWorkExperience(tempString);
                break;
            case 14:
                jobToAdd.setTraining(tempString);
                break;
            case 15:
                jobToAdd.setHandbookContent(tempString);
                jobToAdd.setJobIndex(currentSize);
                // adds job to database at the end of every table row
                addJobToDatabase(jobToAdd);
                columnCount = -1;
                currentSize++;
                break;
            default:
                break;
        }
        // skips matrix and handbook columns in file and finds the longest std::string from input to adjust for column size
        if (columnCount != 1 && columnCount != 15 && tempString.size() + 1 > columnLengths[columnCount]) {
            columnLengths[columnCount] = tempString.size() + 1;
        }
        // increments columnCount
        columnCount++;
    }
}

void JobDatabase::importList(std::fstream &listData, SinglyLinkedList *list, const HashTable &hashTable) {
    // setting the cursor position back to the start
    listData.clear();
    listData.seekg(0, std::ios::beg);
    std::string initialString;
    std::string tempString;
    int matrixCode;
    Occupation *jobPointer;
    int jobIndex;
    // loop while file does not end
    while (std::getline(listData, tempString)) {
        // try catch block to handle erroneous input
        try {
            initialString = tempString;
            // erases the - character from the matrix code
            auto pos = tempString.find("-");
            if (pos != std::string::npos) {
                tempString.erase(pos, 1);
            }
            // converts the modified code to an int
            matrixCode = stoi(tempString);
            // searches for that code in the string
            jobPointer = hashTable.getJobPointer(matrixCode);
            if (jobPointer) {
                list->append(jobPointer);
            } else {
                std::cout << "\nFailed to find occupation with matrix code " << initialString << std::endl;
            }
        } catch (std::exception&) {
            std::cout << "\nSkipping " << initialString << " from list input file." << std::endl;
        }
    }
}

void JobDatabase::rewriteListFile(std::fstream &listData, SinglyLinkedList *list) {
    listData.clear();
    listData.seekp(0, std::ios::beg);    SinglyLinkedNode<Occupation> *current = list->getListHead();
    // iterates through the list and outputs the matrix codes
    while (current) {
        listData << current->data.getMatrixCode() << std::endl;
        current = current->next;
    }
}

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

bool JobDatabase::searchArrayByJob(DynamicArray<Occupation*> &searchedJobsArray, const std::string &jobToSearch) const{

    int mainArraySize = allJobsArray.getCurrentSize();
    searchedJobsArray = DynamicArray<Occupation*>(mainArraySize);

    // linear search for substrings
    for (int i = 0; i < mainArraySize; i++) {
        std::string query = jobToSearch;
        std::string currentEntry = allJobsArray[i].getOccupation();
        lowerString(query);
        lowerString(currentEntry);

        if (currentEntry.find(query) != std::string::npos) {
            searchedJobsArray.addJobToArray(allJobsArray[i]);
        }
    }

    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}

bool JobDatabase::searchArrayByWage(DynamicArray<Occupation *> &searchedJobsArray, const float &lowerLimit, const float &upperLimit) const {

    int mainArraySize = allJobsArray.getCurrentSize();
    searchedJobsArray = DynamicArray<Occupation*>(mainArraySize);

    // linear search to find whether wage sits between the upper and lower bounds inclusive
    for (int i = 0; i < mainArraySize; i++) {
        if (allJobsArray[i].getWage() >= lowerLimit && allJobsArray[i].getWage() <= upperLimit) {
            searchedJobsArray.addJobToArray(allJobsArray[i]);
        }
    }

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

void JobDatabase::addJobToDatabase(const Occupation &jobToAdd) {
    allJobsHashTable.insertJob(allJobsArray.addJobToArray(jobToAdd));
}




