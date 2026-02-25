#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Helpers.h"
#include "DynamicArray.h"

DynamicArray::DynamicArray() : capacity(0), data(nullptr) {}

DynamicArray::DynamicArray(int capacity) : capacity(capacity) {
    data = new Occupation[capacity];
}

DynamicArray& DynamicArray::operator=(const DynamicArray &other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new Occupation[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

Occupation &DynamicArray::operator[](int index) const {
    return data[index];
}

DynamicArray::~DynamicArray() {
    delete[] data;
    data = nullptr;
}

void DynamicArray::increaseCapacity() {
    if (currentSize < capacity) return;

    int newCapacity = (capacity == 0) ? 1 : capacity * 2;
    Occupation *newData = new Occupation[newCapacity];

    for (int i = 0; i < currentSize; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

int DynamicArray::getCurrentSize() const {
    return currentSize;
}

int DynamicArray::getCapacity() const {
    return capacity;
}

void DynamicArray::readEntries(std::ifstream &rawData, const std::string *headings,
                               int *columnLengths) {
    // code by ChatGPT to reset file stream cursor position after first while std::getline function
    rawData.clear();
    rawData.seekg(0, std::ios::beg);
    // temporary std::string copy strings from data to struct variables
    std::string tempString = "-";
    // column counter
    short columnCount = 0;

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
                this->data[currentSize].setOccupation(tempString);
                break;
            case 1:
                this->data[currentSize].setMatrixCode(tempString);
                break;
            case 2:
                this->data[currentSize].setOccupationType(tempString);
                break;
            case 3:
                this->data[currentSize].setEmploymentCurrent(tempString);
                break;
            case 4:
                this->data[currentSize].setEmploymentFuture(tempString);
                break;
            case 5:
                this->data[currentSize].setDistributionCurrent(tempString);
                break;
            case 6:
                this->data[currentSize].setDistributionFuture(tempString);
                break;
            case 7:
                this->data[currentSize].setNumericChange(tempString);
                break;
            case 8:
                this->data[currentSize].setPercentageChange(tempString);
                break;
            case 9:
                this->data[currentSize].setPercentSelfEmployed(tempString);
                break;
            case 10:
                this->data[currentSize].setJobOpenings(tempString);
                break;
            case 11:
                this->data[currentSize].setWage(tempString);
                break;
            case 12:
                this->data[currentSize].setEducation(tempString);
                break;
            case 13:
                this->data[currentSize].setWorkExperience(tempString);
                break;
            case 14:
                this->data[currentSize].setTraining(tempString);
                break;
            case 15:
                this->data[currentSize].setHandbookContent(tempString);
                this->data[currentSize].setJobIndex(currentSize);
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

int DynamicArray::searchByJob(const DynamicArray &allJobs, const std::string &jobSearched) {
    // initializing counter
    int foundSearches = 0;
    int allJobsSize = allJobs.getCurrentSize();

    // linear search for substrings
    for (int i = 0; i < allJobsSize; i++) {
        std::string query = jobSearched;
        std::string currentEntry = allJobs.data[i].getOccupation();
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);
        std::transform(currentEntry.begin(), currentEntry.end(), currentEntry.begin(), ::tolower);

        if (currentEntry.find(query) != std::string::npos) {
            this->data[foundSearches] = allJobs.data[i];
            foundSearches++;
            currentSize++;
        }
    }

    return foundSearches;
}

int DynamicArray::searchByWage(const DynamicArray &allJobs, const int &jobCounter, const float &lowerLimit,
                               const float &upperLimit) {
    // initializing counter
    int foundSearches = 0;

    // linear search to find whether wage sits between the upper and lower bounds inclusive
    for (int i = 0; i < jobCounter; i++) {
        if (allJobs.data[i].getWage() >= lowerLimit && allJobs.data[i].getWage() <= upperLimit) {
            this->data[foundSearches] = allJobs.data[i];
            foundSearches++;
        }
    }
    return foundSearches;
}

Occupation DynamicArray::addJobToArray(const Occupation &jobToAdd) {
    data[currentSize] = jobToAdd;
    currentSize++;
    return data[currentSize - 1];
}

void DynamicArray::addEntryAgain(Occupation jobAdded, DynamicArray allJobs, int &jobCounter) {
    int indexAdded = jobAdded.getJobIndex();
    // add entry shifts elements to the right
    for (int i = jobCounter - 1; i >= indexAdded; i--) {
        // starts from the end of the array and shifts elements to the right (suggested by ChatGPT)
        allJobs[i].setJobIndex(allJobs[i].getJobIndex() + 1);
        allJobs[i + 1] = allJobs[i];
    }
    allJobs[indexAdded] = jobAdded;
    // increments jobCounter to represent increasing number of jobs in the database
    jobCounter++;
}

Occupation DynamicArray::removeEntry(DynamicArray allJobs, int &jobCounter, int &totalJobsCapacity, int indexRemoved) {
    Occupation jobToReturn = allJobs[indexRemoved];
    // remove entry shifts elements to the left instead of resizing
    for (int i = indexRemoved + 1; i < jobCounter; i++) {
        // decrements jobIndex datafield of each job to reflect shifting elements to the left
        allJobs[i].setJobIndex(allJobs[i].getJobIndex() - 1);
        allJobs[i - 1] = allJobs[i];
    }
    // decrements jobCounter to represent decreasing number of jobs in the database
    jobCounter--;
    // returns occupation being removed
    return jobToReturn;
}

void DynamicArray::rewriteJobFile(Occupation *allJobs, const int &jobCounter) {
    // opens the input file as an output file stream and automatically wipes the file's contents
    std::ofstream modifiedData("../input/rawData.txt");
    // iterates through entire dynamic array
    for (int i = 0; i < jobCounter; i++) {
        modifiedData << allJobs[i].getOccupation() << std::endl
                << allJobs[i].getMatrixCode() << std::endl
                << allJobs[i].getOccupationType() << std::endl
                << allJobs[i].getEmploymentCurrentString() << std::endl
                << allJobs[i].getEmploymentFutureString() << std::endl
                << allJobs[i].getDistributionCurrentString() << std::endl
                << allJobs[i].getDistributionFutureString() << std::endl
                << allJobs[i].getNumericChangeString() << std::endl
                << allJobs[i].getPercentageChangeString() << std::endl
                << allJobs[i].getPercentSelfEmployedString() << std::endl
                << allJobs[i].getJobOpeningsString() << std::endl
                << allJobs[i].getWageString() << std::endl
                << allJobs[i].getEducation() << std::endl
                << allJobs[i].getWorkExperience() << std::endl
                << allJobs[i].getTraining() << std::endl
                << allJobs[i].getHandbookContent() << std::endl;
    }
    // closes file
    modifiedData.close();
}

void DynamicArray::importList(std::ifstream &listData, Occupation *allJobs, SinglyLinkedList *list, int jobCounter,
                              HashTable &hashTable) {
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
            tempString.erase(2, 1);
            // converts the modified code to an int
            matrixCode = stoi(tempString);
            // searches for that code in the string
            jobPointer = hashTable.getJobPointer(matrixCode);
            if (jobPointer) {
                list->append(&allJobs[jobPointer->getJobIndex()]);
            } else {
                std::cout << "\nFailed to find occupation with matrix code " << initialString << std::endl;
            }
        } catch (...) {
            std::cout << "\nSkipping " << initialString << " from list input file." << std::endl;
        }
    }
}

void DynamicArray::rewriteListFile(SinglyLinkedList *list) {
    // opens the input file as an output file stream and automatically wipes the file's contents
    std::ofstream modifiedData("../input/listData.txt");
    SinglyLinkedNode<Occupation> *current = list->getListHead();
    // iterates through the list and outputs the matrix codes
    while (current) {
        modifiedData << current->data.getMatrixCode() << std::endl;
        current = current->next;
    }
    // closes file
    modifiedData.close();
}
