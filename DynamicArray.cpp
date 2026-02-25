#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Helpers.h"
#include "DynamicArray.h"

template<typename T>
DynamicArray<T>::DynamicArray(int capacity) : capacity(capacity) {
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray &&other) noexcept{
    if (this != &other) {
        delete[] data;

        capacity = other.capacity;
        currentSize = other.currentSize;
        data = other.data;

        other.capacity = 0;
        other.currentSize = 0;
        other.data = nullptr;
    }

    return *this;
}

template <typename T>
Occupation &DynamicArray<T>::operator[](int index) const {
    return *data[index];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
    data = nullptr;
}

template <typename T>
void DynamicArray<T>::increaseCapacity() {
    if (currentSize < capacity) return;

    const int newCapacity = (capacity == 0) ? 1 : capacity * 2;
    auto newData = new std::unique_ptr<Occupation>[newCapacity];

    for (int i = 0; i < currentSize; i++) {
        newData[i] = std::move(data[i]);
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
int DynamicArray<T>::getCurrentSize() const {
    return currentSize;
}

template <typename T>
int DynamicArray<T>::getCapacity() const {
    return capacity;
}

template <typename T>
void DynamicArray<T>::readInputFile(std::fstream &rawData, const std::string *headings,
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
                this->data[currentSize]->setOccupation(tempString);
                break;
            case 1:
                this->data[currentSize]->setMatrixCode(tempString);
                break;
            case 2:
                this->data[currentSize]->setOccupationType(tempString);
                break;
            case 3:
                this->data[currentSize]->setEmploymentCurrent(tempString);
                break;
            case 4:
                this->data[currentSize]->setEmploymentFuture(tempString);
                break;
            case 5:
                this->data[currentSize]->setDistributionCurrent(tempString);
                break;
            case 6:
                this->data[currentSize]->setDistributionFuture(tempString);
                break;
            case 7:
                this->data[currentSize]->setNumericChange(tempString);
                break;
            case 8:
                this->data[currentSize]->setPercentageChange(tempString);
                break;
            case 9:
                this->data[currentSize]->setPercentSelfEmployed(tempString);
                break;
            case 10:
                this->data[currentSize]->setJobOpenings(tempString);
                break;
            case 11:
                this->data[currentSize]->setWage(tempString);
                break;
            case 12:
                this->data[currentSize]->setEducation(tempString);
                break;
            case 13:
                this->data[currentSize]->setWorkExperience(tempString);
                break;
            case 14:
                this->data[currentSize]->setTraining(tempString);
                break;
            case 15:
                this->data[currentSize]->setHandbookContent(tempString);
                this->data[currentSize]->setJobIndex(currentSize);
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

template <typename T>
int DynamicArray<T>::searchByJob(const DynamicArray &allJobs, const std::string &jobSearched) {
    // initializing counter
    int foundSearches = 0;
    int allJobsSize = allJobs.getCurrentSize();

    // linear search for substrings
    for (int i = 0; i < allJobsSize; i++) {
        std::string query = jobSearched;
        std::string currentEntry = allJobs.data[i]->getOccupation();
        lowerString(query);
        lowerString(currentEntry);

        if (currentEntry.find(query) != std::string::npos) {
            this->data[foundSearches] = allJobs.data[i];
            foundSearches++;
            currentSize++;
        }
    }

    return foundSearches;
}

template <typename T>
int DynamicArray<T>::searchByWage(const DynamicArray &allJobs, const int &jobCounter, const float &lowerLimit,
                               const float &upperLimit) {
    // initializing counter
    int foundSearches = 0;

    // linear search to find whether wage sits between the upper and lower bounds inclusive
    for (int i = 0; i < jobCounter; i++) {
        if (allJobs.data[i]->getWage() >= lowerLimit && allJobs.data[i].getWage() <= upperLimit) {
            this->data[foundSearches] = allJobs.data[i];
            foundSearches++;
        }
    }
    return foundSearches;
}

template <typename T>
Occupation DynamicArray<T>::addJobToArray(const Occupation &jobToAdd) {
    data[currentSize] = jobToAdd;
    currentSize++;
    return data[currentSize - 1];
}

template <typename T>
void DynamicArray<T>::addEntryAgain(const Occupation &jobAdded) {
    int indexAdded = jobAdded.getJobIndex();
    // iterates from the last job in the array (currentSize always < capacity in this case)
    for (int i = currentSize - 1; i >= indexAdded; i--) {
        data[i].setJobIndex(data[i].getJobIndex() + 1);
        data[i + 1] = data[i];
    }
    data[indexAdded] = jobAdded;
    currentSize++;
}

template <typename T>
Occupation DynamicArray<T>::removeEntry(const int indexRemoved) {
    // remove entry and shifts elements to the left
    for (int i = indexRemoved + 1; i < currentSize; i++) {
        data[i].setJobIndex(data[i].getJobIndex() - 1);
        data[i - 1] = data[i];
    }

    currentSize--;
    return data[indexRemoved];
}

template <typename T>
void DynamicArray<T>::rewriteInputFile(std::fstream &modifiedData) {
    for (int i = 0; i < currentSize; i++) {
        modifiedData << data[i].getOccupation() << std::endl
                << data[i].getMatrixCode() << std::endl
                << data[i].getOccupationType() << std::endl
                << data[i].getEmploymentCurrentString() << std::endl
                << data[i].getEmploymentFutureString() << std::endl
                << data[i].getDistributionCurrentString() << std::endl
                << data[i].getDistributionFutureString() << std::endl
                << data[i].getNumericChangeString() << std::endl
                << data[i].getPercentageChangeString() << std::endl
                << data[i].getPercentSelfEmployedString() << std::endl
                << data[i].getJobOpeningsString() << std::endl
                << data[i].getWageString() << std::endl
                << data[i].getEducation() << std::endl
                << data[i].getWorkExperience() << std::endl
                << data[i].getTraining() << std::endl
                << data[i].getHandbookContent() << std::endl;
    }
    // closes file
    modifiedData.close();
}

template <typename T>
void DynamicArray<T>::importList(std::fstream &listData, Occupation *allJobs, SinglyLinkedList *list, int jobCounter,
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
                list->append(jobPointer);
            } else {
                std::cout << "\nFailed to find occupation with matrix code " << initialString << std::endl;
            }
        } catch (...) {
            std::cout << "\nSkipping " << initialString << " from list input file." << std::endl;
        }
    }
}

template <typename T>
void DynamicArray<T>::rewriteListFile(SinglyLinkedList *list) {
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
