#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Helpers.h"
#include "DynamicArray.h"

DynamicArray::DynamicArray(int &capacity) {
    data = new Occupation[capacity];
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


void DynamicArray::readEntries(std::ifstream &rawData, const std::string *headings,
                 int *columnLengths, int &jobCounter) {
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
                this->data[currentSize].setJobIndex(jobCounter);
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

void DynamicArray::viewEntries(const std::string* headings, int* columnLengths, const int& jobCounter)
const {
    // output to text file
    std::ofstream output("../output/output.txt");

    // outputs table headings
    printTableHeadings(output, headings, columnLengths);
    // outputs contents of the table
    for (int i = 0; i < currentSize; i++) {
        // pass in each instance to the function to print
        printTableEntry(output, columnLengths, this->data[i]);
    }
    // closes output file
    output.close();
}

int DynamicArray::searchByJob(const DynamicArray& allJobs, const int& jobCounter, std::string& jobSearched) {
    // initializing counter
    int foundSearches = 0;

    // linear search for substrings
    for (int i = 0; i < jobCounter; i++) {

        std::string query = jobSearched;
        std::string currentEntry = allJobs.data[i].getOccupation();
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);
        std::transform (currentEntry.begin(), currentEntry.end(), currentEntry.begin(), ::tolower);

        if (currentEntry.find(query) != std::string::npos) {
            this->data[foundSearches] = allJobs.data[i];
            foundSearches++;
        }
    }

    return foundSearches;
}

int DynamicArray::searchByWage(const DynamicArray& allJobs, const int& jobCounter, const float& lowerLimit,
        const float& upperLimit) {
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

Occupation DynamicArray::addEntry(int &jobCounter, int &totalJobsCapacity, std::string &jobAdded,
                                  int *columnLengths, std::string key) {
    // temporary std::string and float to store user input
    std::string tempString = "";
    float tempFloat = 0.0f;

    // Code from zyBooks to increase the capacity of an array
    if (currentSize == capacity) {
        increaseCapacity();
    }

    // Sets occupation based on previous user input
    jobAdded[0] = toupper(jobAdded[0]);
    this->data[jobCounter].setOccupation(jobAdded + " *");

    this->data[jobCounter].setMatrixCode(key);

    // Gives user option to set occupation type as line item or summary
    std::cout << "\nEnter A for a summary occupation (a larger group for other jobs)." << std::endl
            << "Enter B for a line-item occupation (a single job).\n" << std::endl;
    std::getline(std::cin, tempString);
    tempString[0] = toupper(tempString[0]);
    // Error handling while loop
    while (tempString != "A" && tempString != "B") {
        std::cout << "\nEnter A or B.\n" << std::endl;
        std::getline(std::cin, tempString);
        tempString[0] = toupper(tempString[0]);
    }
    switch (menuHandling('A', 'B')) {
        case 'A': // assigns "Summary" occupation type
            this->data[jobCounter].setOccupationType("Summary");
            break;
        case 'B': // assigns "Line item" occupation type
            this->data[jobCounter].setOccupationType("Line item");
            break;
        default:
            break;
    }

    // Takes std::string, converts it to float for calculation, and converts it to std::string again to pass as argument
    std::cout << "\nEnter employment number in 2023:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::getline(std::cin, tempString);
    tempFloat = toFloat(tempString);
    // checks for negative numbers or characters
    // chatGPT suggested using the g_catch value to initialize the loop and assigning toFloat(tempString) to a variable
    while (g_catch == true || tempFloat < 0.0f) {
        // passes "-" to setter
        if (tempString == "-") {
            this->data[jobCounter].setEmploymentCurrent("-");
            break;
        }
        // asks user for input if they inputted characters or negative values
        else {
            std::cout << "\nEnter a non-negative value for employment number." << std::endl
                    << "Enter '-' if unknown.\n" << std::endl;
            std::getline(std::cin, tempString);
            tempFloat = toFloat(tempString);
        }
    }
    // calculates if tempString is not "-"
    if (tempString != "-") {
        tempFloat = std::stof(tempString) / 1000;
        std::stringstream roundedString;
        // ChatGPT and Google AI suggested multiplying by ten, using round(), and dividing to round to 1 d.p.
        // ChatGPT suggested using std::string stream to work around 6 decimal place output of round() for a floating point
        roundedString << std::fixed << std::setprecision(1) << std::round(tempFloat * 10) / 10.0f;
        tempString = roundedString.str();
        allJobs.data[jobCounter].setEmploymentCurrent(tempString);
        // ChatGPT suggested clearing std::string. This fixed the issue of the std::string not being deleted before accepting another input
        roundedString.str("");
        roundedString.clear();
    }

    std::cout << "\nEnter employment projection for 2033:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::getline(std::cin, tempString);
    tempFloat = toFloat(tempString);
    // checks for negative numbers or characters
    while (g_catch == true || tempFloat < 0.0f) {
        // passes "-" to setter
        if (tempString == "-") {
            allJobs.data[jobCounter].setEmploymentFuture("-");
            allJobs.data[jobCounter].setNumericChange("-");
            allJobs.data[jobCounter].setPercentageChange("-");
            break;
        }
        // asks user for input if they inputted characters or negative values
        else {
            std::cout << "\nEnter a non-negative value for employment projection." << std::endl
                    << "Enter '-' if unknown.\n" << std::endl;
            std::getline(std::cin, tempString);
            tempFloat = toFloat(tempString);
        }
    }
    // calculates if tempString is not "-"
    if (tempString != "-") {
        // rounding user input to 1 decimal place
        std::stringstream roundedString;
        tempFloat = stof(tempString) / 1000;
        roundedString << std::fixed << std::setprecision(1) << round(tempFloat * 10) / 10.0f;
        tempString = roundedString.str();
        allJobs.data[jobCounter].setEmploymentFuture(tempString);
        roundedString.str("");
        roundedString.clear();

        // Sets numeric change by subtracting current employment from future employment
        tempFloat = allJobs.data[jobCounter].getEmploymentFuture() - allJobs.data[jobCounter].getEmploymentCurrent();
        roundedString << std::fixed << std::setprecision(1) << tempFloat;
        tempString = roundedString.str();
        allJobs.data[jobCounter].setNumericChange(tempString);
        roundedString.str("");
        roundedString.clear();

        // Sets percentage change by using the percentage change formula
        tempFloat = ((allJobs.data[jobCounter].getEmploymentFuture() - allJobs.data[jobCounter].getEmploymentCurrent())
                     / allJobs[jobCounter].getEmploymentCurrent())
                    * 100;
        roundedString << std::fixed << std::setprecision(1) << round(tempFloat * 10) / 10.0f;
        tempString = roundedString.str();
        allJobs.data[jobCounter].setPercentageChange(tempString);
        roundedString.str("");
        roundedString.clear();
    }

    // Takes std::string as input for job openings
    std::cout << "\nEnter the number of job openings from 2023-2033 per year on average:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::getline(std::cin, tempString);
    tempFloat = toFloat(tempString);
    // checks for negative numbers or characters
    while (g_catch == true || tempFloat < 0.0f) {
        // breaks if user inputs "-"
        if (tempString == "-") {
            break;
        } else {
            std::cout << "\nEnter a non-negative value for job openings." << std::endl
                    << "Enter '-' if unknown.\n" << std::endl;
            std::getline(std::cin, tempString);
            tempFloat = toFloat(tempString);
        }
    }
    allJobs.data[jobCounter].setJobOpenings(tempString);

    // Takes std::string as input for wage
    std::cout << "\nEnter median annual wage in dollars for 2024:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::getline(std::cin, tempString);
    tempFloat = toFloat(tempString);
    // checks for negative numbers or characters
    while (g_catch == true || tempFloat < 0.0f) {
        // breaks if user inputs "-"
        if (tempString == "-") {
            break;
        } else {
            std::cout << "\nEnter a non-negative value for median annual wage." << std::endl
                    << "Enter '-' if unknown.\n" << std::endl;
            std::getline(std::cin, tempString);
            tempFloat = toFloat(tempString);
        }
    }
    allJobs.data[jobCounter].setWage(tempString);

    // Presents options for typical education level and takes in std::string
    std::cout << "\nEnter typical education needed for entry:" << std::endl
            << "A: No formal educational credential" << std::endl
            << "B: High school diploma or equivalent" << std::endl
            << "C: Postsecondary nondegree award" << std::endl
            << "D: Some college, no degree" << std::endl
            << "E: Associate's degree" << std::endl
            << "F: Bachelor's degree" << std::endl
            << "G: Master's degree" << std::endl
            << "H: Doctoral or professional degree" << std::endl
            << "-: Unknown / No Data\n" << std::endl;
    std::getline(std::cin, tempString);
    tempString[0] = toupper(tempString[0]);
    // input handling
    while ((tempString < "A" || tempString > "H") && tempString != "-") {
        std::cout << "\nEnter a letter from A to H or '-'.\n" << std::endl;
        std::getline(std::cin, tempString);
        tempString[0] = toupper(tempString[0]);
    }
    switch (tempString[0]) {
        case 'A':
            allJobs.data[jobCounter].setEducation("No formal educational credential");
            break;
        case 'B':
            allJobs.data[jobCounter].setEducation("High school diploma or equivalent");
            break;
        case 'C':
            allJobs.data[jobCounter].setEducation("Postsecondary nondegree award");
            break;
        case 'D':
            allJobs.data[jobCounter].setEducation("Some college, no degree");
            break;
        case 'E':
            allJobs.data[jobCounter].setEducation("Associate�s degree");
            break;
        case 'F':
            allJobs.data[jobCounter].setEducation("Bachelor�s degree");
            break;
        case 'G':
            allJobs.data[jobCounter].setEducation("Master�s degree");
            break;
        case 'H':
            allJobs.data[jobCounter].setEducation("Doctoral or professional degree");
            break;
        case '-':
            allJobs.data[jobCounter].setEducation("-");
            break;
        default:
            break;
    }

    // Presents options for work experience and takes in std::string
    std::cout << "\nEnter work experience in a related occupation required:" << std::endl
            << "A: None" << std::endl
            << "B: Less than 5 years" << std::endl
            << "C: 5 years or more" << std::endl
            << "-: Unknown / No Data\n" << std::endl;
    std::getline(std::cin, tempString);
    tempString[0] = toupper(tempString[0]);
    // input handling
    while ((tempString < "A" || tempString > "C") && tempString != "-") {
        std::cout << "\nEnter a letter from A to C or '-'.\n" << std::endl;
        std::getline(std::cin, tempString);
        tempString[0] = toupper(tempString[0]);
    }
    switch (tempString[0]) {
        case 'A':
            allJobs.data[jobCounter].setWorkExperience("None");
            break;
        case 'B':
            allJobs.data[jobCounter].setWorkExperience("Less than 5 years");
            break;
        case 'C':
            allJobs.data[jobCounter].setWorkExperience("5 years or more");
            break;
        case '-':
            allJobs.data[jobCounter].setWorkExperience("-");
            break;
        default:
            break;
    }

    // Presents options for on-the-job training and takes in std::string
    std::cout << "\nEnter typical on-the-job training needed to attain competency in the occupation:" << std::endl
            << "A: None" << std::endl
            << "B: Short-term on-the-job training" << std::endl
            << "C: Moderate-term on-the-job training" << std::endl
            << "D: Long-term on-the-job training" << std::endl
            << "E: Apprenticeship" << std::endl
            << "F: Internship/residency" << std::endl
            << "-: Unknown / No Data\n" << std::endl;
    std::getline(std::cin, tempString);
    tempString[0] = toupper(tempString[0]);
    // input handling
    while ((tempString < "A" || tempString > "F") && tempString != "-") {
        std::cout << "\nEnter a letter from A to F or '-'.\n" << std::endl;
        std::getline(std::cin, tempString);
        tempString[0] = toupper(tempString[0]);
    }
    switch (tempString[0]) {
        case 'A':
            allJobs.data[jobCounter].setTraining("None");
            break;
        case 'B':
            allJobs.data[jobCounter].setTraining("Short-term on-the-job training");
            break;
        case 'C':
            allJobs.data[jobCounter].setTraining("Moderate-term on-the-job training");
            break;
        case 'D':
            allJobs.data[jobCounter].setTraining("Long-term on-the-job training");
            break;
        case 'E':
            allJobs.data[jobCounter].setTraining("Apprenticeship");
            break;
        case 'F':
            allJobs.data[jobCounter].setTraining("Internship/residency");
            break;
        case '-':
            allJobs.data[jobCounter].setTraining("-");
            break;
        default:
            break;
    }

    // sets the index of the newly added job
    allJobs.data[jobCounter].setJobIndex(jobCounter);

    // increments jobCounter
    jobCounter++;
    // returns recently added job
    return allJobs.data[jobCounter - 1];
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
