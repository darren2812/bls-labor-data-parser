#include <iostream>
#include <iomanip>
#include <cmath>
#include "MenuHandler.h"
#include "Helpers.h"


MenuHandler::MenuHandler()
    : rawDataPath("input/rawData.txt"),
      listDataPath("input/listData.txt"),
      outputPath("output/output.txt"),
      rawData(rawDataPath),
      listData(listDataPath),
      output(outputPath) {
    int numberOfRows = 0;
    std::string dummy;

    // sets default table column lengths to the size of the headings + 1
    for (int i = 0; i < NUM_OF_HEADINGS; i++) {
        tableColumnLengths[i] = tableHeadings[i].size() + 1;
    }

    if (!rawData.is_open()) {
        std::cout << "\nThe file rawData.txt could not be opened. Exiting the program..." << std::endl;
        return;
    }
    if (!listData.is_open()) {
        std::cout << "\nThe file listData.txt could not be opened" << std::endl;
    }
    if (!output.is_open()) {
        std::cout << "\nThe file output.txt could not be opened" << std::endl;
    }

    // count number of lines and assign to dummy string
    while (std::getline(rawData, dummy)) {
        numberOfRows++;
    }

    if (numberOfRows % NUM_OF_HEADINGS != 0) {
        std::cout <<
                "\nThe number of rows in the input file are incompatible. Try redownloading the input source file.\n" <<
                std::endl
                << "Exiting the program..." << std::endl;
        return;
    }

    totalJobsCapacity = numberOfRows / NUM_OF_HEADINGS;
}

MenuHandler::~MenuHandler() {
    rawData.close();
    listData.close();
    output.close();
}

void MenuHandler::allocateDatabase() {
    allJobsDatabase = JobDatabase(totalJobsCapacity);
}


void MenuHandler::displayMainMenu() {
    std::cout << "\nWelcome. Input a letter to the console to execute an operation below." << std::endl
            << "A: View all Data Entries" << std::endl
            << "B: Search for Data Entries" << std::endl
            << "C: Add a Data Entry" << std::endl
            << "D: Remove a Data Entry" << std::endl
            << "E: View Recent Changes" << std::endl
            << "F: Undo Most Recent Change" << std::endl
            << "G: Save Changes to Jobs Input File" << std::endl
            << "H: Manage your Occupation List" << std::endl
            << "I: Compare Occupation Statistics" << std::endl
            << "J: Exit the Program\n" << std::endl;
}

void MenuHandler::displayListMenu() {
    std::cout << "\nInput a letter to manage your LIST." << std::endl
            << "A: View Your Current List" << std::endl
            << "B: Search for an Occupation in Your List" << std::endl
            << "C: Add an Existing Occupation to Your List" << std::endl
            << "D: Remove an Occupation from Your List " << std::endl
            << "E: View Recent Changes" << std::endl
            << "F: Undo Most Recent Change to Your List" << std::endl
            << "G: Save Changes to List Input File" << std::endl
            << "H: Clear Your Existing List" << std::endl
            << "I: Return to Main Menu\n" << std::endl;
}

void MenuHandler::run() {
    allocateDatabase();
    allJobsDatabase.readInputFile(rawData, tableColumnLengths);
    allJobsDatabase.readListFile(listData, jobsList);

    bool isRunning = true;

    while (isRunning) {
        displayMainMenu();
        char input = menuHandling('A', 'J', false);

        switch (input) {
            case 'A':
                handleDatabasePrint();
                break;
            case 'B':
                handleSearch(Structure::MAIN_DATABASE);
                break;
            case 'C':
                handleAddDatabase();
                break;
            case 'D':
                handleRemoveDatabase();
                break;
            case 'E':
                handleStackPrint(recentChangesDatabase, Structure::MAIN_DATABASE);
                break;
            case 'F':
                handleUndoDatabase();
                break;
            case 'G':
                allJobsDatabase.rewriteInputFile(rawDataPath, rawData);
                savedDatabase = true;
                std::cout << "\nChanges saved to file." << std::endl;
                break;
            case 'H':
                runListMenu();
                break;
            case 'I':
                handleCompare();
                break;
            case 'J':
                // handleCheckSaved returns true if all changes have been saved
                if (handleCheckSaved()) {
                    isRunning = false;
                } else {
                    isRunning = true;
                }
                break;
            default:
                break;
        }
    }
    std::cout << "\nGoodbye!" << std::endl;
}

void MenuHandler::runListMenu() {
    bool isRunning = true;

    while (isRunning) {
        displayListMenu();
        char input = menuHandling('A', 'I', false);

        switch (input) {
            case 'A':
                handleListPrint();
                break;
            case 'B':
                handleSearch(Structure::LIST);
                break;
            case 'C':
                handleAddList();
                break;
            case 'D':
                handleRemoveList();
                break;
            case 'E':
                handleStackPrint(recentChangesList, Structure::LIST);
                break;
            case 'F':
                handleUndoList();
                break;
            case 'G':
                allJobsDatabase.rewriteListFile(listDataPath, listData, jobsList);
                savedList = true;
                std::cout << "\nChanges saved to list file." << std::endl;
                break;
            case 'H':
                handleClearList();
                break;
            case 'I':
                isRunning = false;
                break;
            default:
                break;
        }
    }
}

// function to print headings
void MenuHandler::printTableHeadings() {
    output.clear();
    output.seekp(0, std::ios::beg);
    // total width of the table in output and console
    int totalWidth = 0;
    int consoleWidth = 0;

    // outputs table title in output file and console
    output << "Table 1.2 Occupational projections, 2023-2033, and worker characteristics, 2023"
            << " (Numbers in thousands, except percentages and median annual wages)" << std::endl
            << "An * after an occupation title represents a user-created occupation.\n" << std::endl
            << "Link to Employment Data Definitions: https://www.bls.gov/emp/documentation/definitions.htm" << std::endl
            << "Link to Original Dataset: https://www.bls.gov/emp/tables/occupational-projections-and-characteristics.htm\n"
            << std::endl;
    std::cout << "\nTable 1.2 Summary (Employment number in thousands)" << std::endl
            << "An * after an occupation title represents a user-created occupation.\n" << std::endl
            << "Link to Employment Data Definitions: https://www.bls.gov/emp/documentation/definitions.htm" << std::endl
            << "Link to Original Dataset: https://www.bls.gov/emp/tables/occupational-projections-and-characteristics.htm\n"
            << std::endl;

    // outputs headings to the output file and skips over column 2 and 16
    for (int i = 0; i < NUM_OF_HEADINGS; i++) {
        if (i != 1 && i != 15) {
            output << std::left << std::setw(static_cast<int>(tableColumnLengths[i])) << tableHeadings[i] << "|";
            totalWidth += tableColumnLengths[i];
            // takes widths from job title, employment, wage, education, and work experience columns for console summary
            if (i == 0 || i == 3 || i == 11 || i == 12 || i == 13) {
                std::cout << std::left << std::setw(tableColumnLengths[i]) << tableHeadings[i] << "|";
                consoleWidth += tableColumnLengths[i];
            }
        }
    }
    // moves cursor down to next line
    std::cout << std::endl;
    output << std::endl;
    // outputs horizontal heading divider for totalWidth + 12 times because '|' character is not counted for totalWidth
    for (int i = 0; i <= totalWidth + 12; i++) {
        output << '=';
    }
    // outputs line at the end of the table
    output << '|' << std::endl;
    // similar loop but outputs to console instead
    for (int i = 0; i <= consoleWidth + 3; i++) {
        std::cout << '=';
    }
    std::cout << '|' << std::endl;
}

// function to print table entry
void MenuHandler::printTableEntry(const Occupation *currentJob) {
    // outputs each line of table to the output file
    output << std::left << std::setw(tableColumnLengths[0]) << currentJob->getOccupation() << "|"
            << std::setw(tableColumnLengths[2]) << currentJob->getOccupationType() << "|"
            << std::setw(tableColumnLengths[3]) << currentJob->getEmploymentCurrentString() << "|"
            << std::setw(tableColumnLengths[4]) << currentJob->getEmploymentFutureString() << "|"
            << std::setw(tableColumnLengths[5]) << currentJob->getDistributionCurrentString() << "|"
            << std::setw(tableColumnLengths[6]) << currentJob->getDistributionFutureString() << "|"
            << std::setw(tableColumnLengths[7]) << currentJob->getNumericChangeString() << "|"
            << std::setw(tableColumnLengths[8]) << currentJob->getPercentageChangeString() << "|"
            << std::setw(tableColumnLengths[9]) << currentJob->getPercentSelfEmployedString() << "|"
            << std::setw(tableColumnLengths[10]) << currentJob->getJobOpeningsString() << "|"
            << std::setw(tableColumnLengths[11]) << currentJob->getWageString() << "|"
            << std::setw(tableColumnLengths[12]) << currentJob->getEducation() << "|"
            << std::setw(tableColumnLengths[13]) << currentJob->getWorkExperience() << "|"
            << std::setw(tableColumnLengths[14]) << currentJob->getTraining() << "|" << std::endl;
    // outputs each line of table to the console
    std::cout << std::left << std::setw(tableColumnLengths[0]) << currentJob->getOccupation() << "|"
            << std::setw(tableColumnLengths[3]) << currentJob->getEmploymentCurrentString() << "|"
            << std::setw(tableColumnLengths[11]) << currentJob->getWageString() << "|"
            << std::setw(tableColumnLengths[12]) << currentJob->getEducation() << "|"
            << std::setw(tableColumnLengths[13]) << currentJob->getWorkExperience() << "|" << std::endl;
}

void MenuHandler::printPrefixAndCategory(const Occupation &jobCategory) const {
    std::cout << jobCategory.getMatrixPrefix()
            << ": " << jobCategory.getOccupation() << std::endl;
}

void MenuHandler::printSuffixAndJob(const Occupation &job) const {
    std::cout << job.getMatrixSuffix()
            << ": " << job.getOccupation() << std::endl;
}

void MenuHandler::printMainArray() {
    printTableHeadings();

    allJobsDatabase.forEachJobInMainArray([&](const Occupation *job) {
        printTableEntry(job);
    });
}

void MenuHandler::printHashTable() {
    printTableHeadings();

    allJobsDatabase.forEachEntryInHashTable([&](const Occupation *job) {
        printTableEntry(job);
    });
}

void MenuHandler::printSearchSortResults(DynamicArray<const Occupation *> &array) {
    int arraySize = array.getCurrentSize();

    // outputs table headings
    printTableHeadings();

    // passes the print table entry function to the iterator function in the database class
    for (int i = 0; i < arraySize; i++) {
        printTableEntry(array[i]);
    }
}

void MenuHandler::printEntireStack(const JobStack &stack, const Structure dataset) const {
    std::string datasetString;
    if (dataset == Structure::MAIN_DATABASE) {
        datasetString = "Main Database";
    } else {
        datasetString = "List";
    }

    // making sure that the stack is not empty
    if (stack.getCurrentLength() == 0) {
        std::cout << "\nThere are no changes to display at the moment." << std::endl;
        return;
    }
    const int firstColumnLength = tableColumnLengths[0];
    const int secondColumnLength = 8;
    std::string state;

    std::cout << "\nRecent Changes Made to " << datasetString << "\n" << std::endl;

    std::cout << std::left << std::setw(firstColumnLength) << "Occupation" << "|";
    // 6 is the length of the word 'removed'
    std::cout << std::left << std::setw(secondColumnLength) << "Action" << "|\n";

    // printing heading horizontal divider
    for (int i = 0; i <= firstColumnLength + secondColumnLength; i++) {
        std::cout << '=';
    }
    std::cout << '|' << std::endl;

    stack.forEachJobInStack([&](const JobPair &pair) {
        if (pair.recentState == RecentState::ADDED) {
            state = "Added";
        } else {
            state = "Removed";
        }
        std::string jobTitle;
        // prints out the job removed by searching with matrix code
        if (dataset == Structure::MAIN_DATABASE && pair.recentState == RecentState::REMOVED) {
            jobTitle = pair.job->getOccupation();
        } else {
            const Occupation *job = allJobsDatabase.searchJobByCode(pair.matrixCodeInt);
            if (job != nullptr) {
                jobTitle = job->getOccupation();
            } else {
                jobTitle = "Job no longer in main database.";
            }
        }
        std::cout << std::left << std::setw(firstColumnLength) << jobTitle << "|"
                << std::setw(secondColumnLength) << state << "|" << std::endl;
    });
}

// function to print bar chart to the console
void MenuHandler::printBarChart(float maxValue, const float maxChartLength,
                                const std::function<float(const Occupation *)> &fn) {
    int jobsToCompare = comparedJobsArray.getCurrentSize();
    float valueOfHashTag = maxValue / maxChartLength;

    std::cout << "One # represents " << valueOfHashTag << std::endl;
    for (int j = 0; j < jobsToCompare; j++) {
        std::cout << "[" << j << "]: ";
        int numberOfHashTags = std::round(fn(comparedJobsArray[j]) / valueOfHashTag);
        for (int k = 0; k < numberOfHashTags; k++) {
            std::cout << "#";
        }
        std::cout << " (" << fn(comparedJobsArray[j]) << ")" << std::endl;
    }
}

void MenuHandler::printComparisonResults() {
    int jobsToCompare = comparedJobsArray.getCurrentSize();

    // iterate through all headings but skip ones that are not important to the comparison
    for (int i = 1; i < NUM_OF_HEADINGS - 1; i++) {
        std::cout << "\n\n" << tableHeadings[i] << ":" << std::endl
                << "(Numbers in thousands, except percentages and median annual wages)\n" << std::endl;
        switch (i) {
            case 1:
                for (int j = 0; j < jobsToCompare; j++) {
                    std::cout << "[" << j << "]: " << comparedJobsArray[j]->getMatrixCode() << std::endl;
                }
                break;
            case 2:
                for (int j = 0; j < jobsToCompare; j++) {
                    std::cout << "[" << j << "]: " << comparedJobsArray[j]->getOccupationType() << std::endl;
                }
                break;
            case 3: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getEmploymentCurrent(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 4: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getEmploymentFuture(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 5: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getDistributionCurrent(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 6: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getDistributionFuture(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 7: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getNumericChange(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 8: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getPercentageChange(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 9: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getPercentSelfEmployed(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 10: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getJobOpenings(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 11: {
                // finds the max value and divides it by the max bar chart length for scaling
                auto getterLambda = [](const Occupation *job) { return job->getWage(); };
                printBarChart(findMax(comparedJobsArray, getterLambda),
                              MAX_BAR_CHART_LENGTH,
                              getterLambda);
            }
            break;
            case 12:
                for (int j = 0; j < jobsToCompare; j++) {
                    std::cout << "[" << j << "]: " << comparedJobsArray[j]->getEducation() << std::endl;
                }
                break;
            case 13:
                for (int j = 0; j < jobsToCompare; j++) {
                    std::cout << "[" << j << "]: " << comparedJobsArray[j]->getWorkExperience() << std::endl;
                }
                break;
            case 14:
                for (int j = 0; j < jobsToCompare; j++) {
                    std::cout << "[" << j << "]: " << comparedJobsArray[j]->getTraining() << std::endl;
                }
                break;
            default:
                break;
        }
        std::cout << "\nReference:" << std::endl;
        for (int j = 0; j < jobsToCompare; j++) {
            std::cout << "[" << j << "]: " << comparedJobsArray[j]->getOccupation() << std::endl;
        }
    }
}

void MenuHandler::printAllCategories() const {
    allJobsDatabase.forEachCategory([&](const Occupation *jobCategory) {
        printPrefixAndCategory(*jobCategory);
    });
}

void MenuHandler::printCategoryContents(const std::string &prefix) const {
    allJobsDatabase.forEachJobInCategory(prefix, [&](const Occupation *job) {
        printSuffixAndJob(*job);
    });
}

void MenuHandler::printIndicesInList() const {
    jobsList.forEachJobInList([&](const Occupation *job) {
        std::cout << job->getJobIndex() << ": " << job->getOccupation() << std::endl;
    });
}

// function to handle menu commands
char MenuHandler::menuHandling(char firstLetter, char lastLetter, bool acceptDash) {
    std::string userInput;

    do {
        std::getline(std::cin, userInput);
        if (acceptDash && userInput == "-") {
            return userInput[0];
        }
        capitalizeFirst(userInput);
        if (userInput[0] < firstLetter || userInput[0] > lastLetter) {
            std::cout << "\nEnter a letter from " << firstLetter << " to " << lastLetter << ".\n" << std::endl;
        } else {
            return userInput[0];
        }
    } while (true);
}

// function to handle yes/no commands
char MenuHandler::yesOrNoMenu() {
    std::string userInput = "-";
    std::getline(std::cin, userInput);
    char tempChar = std::tolower(userInput[0]);

    // while loop to handle input
    while (tempChar != 'y' && tempChar != 'n') {
        std::cout << "\nInput y for YES or n for NO\n" << std::endl;
        std::getline(std::cin, userInput);
        tempChar = std::tolower(userInput[0]);
    }

    return tempChar;
}

std::string MenuHandler::promptNonNegativeOrDash() {
    std::string valueToReturn;
    float tempFloat;

    do {
        std::getline(std::cin, valueToReturn);
        try {
            tempFloat = stof(valueToReturn);
            if (tempFloat >= 0.0f) {
                break;
            }
        } catch (const std::exception &) {
            if (valueToReturn == "-") {
                break;
            }
        }
        std::cout << "\nEnter a non-negative value." << std::endl
                << "Enter '-' if unknown.\n" << std::endl;
    } while (true);

    return valueToReturn;
}

OccupationRow MenuHandler::promptJobAttributes(std::string jobTitle, const std::string &matrixCode) {
    // Sets occupation based on previous user input
    capitalizeFirst(jobTitle);
    jobTitle += " *";

    // Gives user option to set occupation type as line item or summary
    std::cout << "\nEnter A for a summary occupation (a larger group for other jobs)." << std::endl
            << "Enter B for a line-item occupation (a single job).\n" << std::endl;
    std::string occupationType;
    switch (menuHandling('A', 'B', false)) {
        case 'A':
            // assigns "Summary" occupation type
            occupationType = "Summary";
            break;
        case 'B':
            // assigns "Line item" occupation type
            occupationType = "Line item";
            break;
        default:
            break;
    }

    // Takes std::string, converts it to float for calculation, and converts it to std::string again to pass as argument
    std::cout << "\nEnter employment number in 2023:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::string employmentCurrent = promptNonNegativeOrDash();

    std::cout << "\nEnter employment projection for 2033:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::string employmentFuture = promptNonNegativeOrDash();

    // Takes std::string as input for job openings
    std::cout << "\nEnter the number of job openings from 2023-2033 per year on average:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::string jobOpenings = promptNonNegativeOrDash();

    // Takes std::string as input for wage
    std::cout << "\nEnter median annual wage in dollars for 2024:" << std::endl
            << "Enter '-' if unknown.\n" << std::endl;
    std::string wage = promptNonNegativeOrDash();

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
    std::string education;
    switch (menuHandling('A', 'H', true)) {
        case 'A':
            education = "No formal educational credential";
            break;
        case 'B':
            education = "High school diploma or equivalent";
            break;
        case 'C':
            education = "Postsecondary nondegree award";
            break;
        case 'D':
            education = "Some college, no degree";
            break;
        case 'E':
            education = "Associate's degree";
            break;
        case 'F':
            education = "Bachelor's degree";
            break;
        case 'G':
            education = "Master's degree";
            break;
        case 'H':
            education = "Doctoral or professional degree";
            break;
        case '-':
            education = "-";
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
    std::string workExperience;
    switch (menuHandling('A', 'C', true)) {
        case 'A':
            workExperience = "None";
            break;
        case 'B':
            workExperience = "Less than 5 years";
            break;
        case 'C':
            workExperience = "5 years or more";
            break;
        case '-':
            workExperience = "-";
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
    std::string training;
    switch (menuHandling('A', 'F', true)) {
        case 'A':
            training = "None";
            break;
        case 'B':
            training = "Short-term on-the-job training";
            break;
        case 'C':
            training = "Moderate-term on-the-job training";
            break;
        case 'D':
            training = "Long-term on-the-job training";
            break;
        case 'E':
            training = "Apprenticeship";
            break;
        case 'F':
            training = "Internship/residency";
            break;
        case '-':
            training = "-";
            break;
        default:
            break;
    }

    OccupationRow row;

    row.occupation = jobTitle;
    row.occupationType = occupationType;
    row.employmentCurrent = employmentCurrent;
    row.employmentFuture = employmentFuture;
    row.jobOpenings = jobOpenings;
    row.wage = wage;
    row.education = education;
    row.workExperience = workExperience;
    row.training = training;
    row.matrixCode = matrixCode;

    return row;
}

std::string MenuHandler::promptNumber() const {
    std::string userInput;

    do {
        std::getline(std::cin, userInput);

        try {
            stoi(userInput);
            return userInput;
        } catch (const std::invalid_argument &) {
            std::cout << "\nThe value entered is not an integer. Try again" << std::endl;
        } catch (const std::out_of_range &) {
            std::cout << "\nThe value entered is out of range. Try again" << std::endl;
        }
    } while (true);
}

char MenuHandler::promptOptionToSearch(Structure dataset) {
    if (dataset == Structure::MAIN_DATABASE) {
        std::cout << "\nWhat do you want to searched based on?" << std::endl
                << "A: Job Title" << std::endl
                << "B: Wage" << std::endl
                << "C: Matrix Code" << std::endl
                << "D: Return to Main Menu\n" << std::endl;
        return menuHandling('A', 'D', false);
    }

    std::cout << "\nWhat do you want to searched based on?" << std::endl
            << "A: Job Title" << std::endl
            << "B: Wage" << std::endl
            << "C: Return to Main Menu\n" << std::endl;
    return menuHandling('A', 'C', false);
}

void MenuHandler::copyMainArray(DynamicArray<const Occupation *> &outputArray) const {
    outputArray = DynamicArray<const Occupation *>(allJobsDatabase.getSize());

    allJobsDatabase.forEachJobInMainArray([&](const Occupation *job) {
        outputArray.append(job);
    });
}

void MenuHandler::copySearchArray(DynamicArray<const Occupation *> &outputArray) const {
    int searchArraySize = searchedJobsArray.getCurrentSize();
    outputArray = DynamicArray<const Occupation *>(searchArraySize);

    for (int i = 0; i < searchArraySize; i++) {
        outputArray.append(searchedJobsArray[i]);
    }
}

void MenuHandler::copyList(DynamicArray<const Occupation *> &outputArray) const {
    int listSize = jobsList.getListSize();
    outputArray = DynamicArray<const Occupation *>(listSize);

    jobsList.forEachJobInList([&](const Occupation *job) {
        outputArray.append(job);
    });
}

// function to pinpoint the specific index of an occupation
const Occupation *MenuHandler::selectSpecificIndex(const std::string &command) {
    std::cout << "\nEnter the name of the occupation to " + command + ".\n" << std::endl;

    std::string userInput;
    std::getline(std::cin, userInput);
    lowerString(userInput);

    allJobsDatabase.searchArrayByJob(searchedJobsArray, userInput);

    std::cout << std::endl;

    int searchedJobsCount = searchedJobsArray.getCurrentSize();
    if (searchedJobsCount > 0) {
        for (int i = 0; i < searchedJobsCount; i++) {
            std::cout << "Index " << searchedJobsArray[i]->getJobIndex() << ": "
                    << searchedJobsArray[i]->getOccupation() << std::endl;
        }
        while (true) {
            std::cout << "\nEnter the index (number) that you want to " << command << ":" << std::endl
                    << "If you want to return to the main menu, enter 'menu'.\n" << std::endl;
            std::getline(std::cin, userInput);
            lowerString(userInput);
            if (userInput == "menu") {
                return nullptr;
            }
            for (int i = 0; i < searchedJobsCount; i++) {
                if (userInput == std::to_string(searchedJobsArray[i]->getJobIndex())) {
                    return searchedJobsArray[i];
                }
            }
        }
    }

    std::cout << "Sorry, we cannot find that job in our database." << std::endl;
    return nullptr;
}

const Occupation *MenuHandler::buildKeyAndSearch() const {
    std::string matrixCode;
    std::string prefix;
    const Occupation *jobCategory;

    do {
        printAllCategories();
        std::cout << "\nThese are the major occupation groups in the database." << std::endl;
        std::cout << "\nEnter a number above to select a category." << std::endl;

        prefix = promptNumber();
        if (prefix == "0") {
            prefix = "00";
        }
        jobCategory = allJobsDatabase.findCategory(prefix);

        if (jobCategory != nullptr) {
            break;
        }
        std::cout << "\nThe category does not exist. Please choose a different category." << std::endl;
    } while (true);

    do {
        printCategoryContents(prefix);
        std::cout << "\nThese are the specific jobs under the " << jobCategory->getOccupation() << " category." <<
                std::endl;
        std::cout << "\nEnter a number above to select a specific occupation." << std::endl;

        std::string suffix = promptNumber();
        if (suffix == "0") {
            suffix = "0000";
        }

        const Occupation *jobToReturn = allJobsDatabase.searchJobByCode(stoi(prefix + suffix));

        if (jobToReturn != nullptr) {
            return jobToReturn;
        }
        std::cout << "\nOccupation not found. Try again." << std::endl;
    } while (true);
}

const Occupation *MenuHandler::chooseJobToModify(const std::string &command) {
    std::cout << "\nDo you want to search for the occupation to " + command + " by title or by matrix code?" <<
            std::endl
            << "A: Title" << std::endl
            << "B: Matrix Code" << std::endl
            << "C: Return to Menu\n" <<
            std::endl;
    switch (menuHandling('A', 'C', false)) {
        case 'A':
            // function returns a pointer to the specific object in the dynamic array
            return selectSpecificIndex(command);
        case 'B':
            // buildKeyAndSearch returns nullptr if user wants to return to the main menu
            return buildKeyAndSearch();
        default:
            return nullptr;
    }
}

bool MenuHandler::placeOccupationInList(const Occupation *occupation) {
    std::cout << "\nWhere in the list would you like to place the occupation?" << std::endl
            << "A: Start of List" << std::endl
            << "B: End of List" << std::endl
            << "C: Middle of List" << std::endl
            << "D: Return to List Menu\n" << std::endl;
    switch (menuHandling('A', 'D', false)) {
        case 'A':
            // prepends object ot the list and prints list
            jobsList.prepend(occupation);
            return true;
        case 'B':
            // appends object to the list and prints list
            jobsList.append(occupation);
            return true;
        case 'C':
            if (jobsList.getListSize() == 0) {
                std::cout << "\nThe list is currently empty. Insert with another method." << std::endl;
                return false;
            }
            jobsList.insertAfter(occupation, handleListIndexRetrieval("add after"));
            return true;
        default:
            return false;
    }
}

bool MenuHandler::databasedIsSaved() {
    return savedDatabase && recentChangesDatabase.getCurrentLength() == 0;
}

bool MenuHandler::listIsSaved() {
    return savedList && recentChangesList.getCurrentLength() == 0;
}


void MenuHandler::handleAddDatabase() {
    std::string jobTitle;

    std::cout << "\nWhat job do you want to add?\n" << std::endl;
    std::getline(std::cin, jobTitle);
    lowerString(jobTitle);

    // outputs existing jobs to console
    if (allJobsDatabase.searchArrayByJob(searchedJobsArray, jobTitle)) {
        std::cout << std::endl;

        printSearchSortResults(searchedJobsArray);

        std::cout << "\nWe have these jobs in our database. Do you still want to add an entry? (y/n)\n" << std::endl;

        switch (yesOrNoMenu()) {
            case 'y':
                break;
            default:
                std::cout << "\nReturning to main menu..." << std::endl;
                return;
        }
    }

    std::string matrixCode;

    do {
        printAllCategories();
        std::string prefix;

        do {
            std::cout << "\nEnter a number above to select a category for the new occupation." << std::endl;
            prefix = promptNumber();

            if (prefix == "0") {
                prefix = "00";
            }
            if (allJobsDatabase.findCategory(prefix)) {
                break;
            }
            std::cout << "\nThe category does not exist. Please choose a different category." << std::endl;
        } while (true);

        if (allJobsDatabase.generateUniqueKey(prefix, matrixCode)) {
            break;
        }

        std::cout << "\nThis category already contains the maximum number of jobs."
                << " Please choose a different category." << std::endl;
    } while (true);

    OccupationRow jobToAdd = promptJobAttributes(jobTitle, matrixCode);
    Occupation *jobToPush = allJobsDatabase.addNewJobToDatabase(jobToAdd);

    recentChangesDatabase.push({nullptr, jobToPush->getJobIndex(), jobToPush->getMatrixCodeInt(), RecentState::ADDED});
    savedDatabase = false;

    // outputs job added to the console
    std::cout << "\n'" << jobToPush->getOccupation()
            << "' is successfully added to the database. Returning to main menu..." << std::endl;
}

int MenuHandler::handleListIndexRetrieval(const std::string &command) {
    do {
        std::cout << "\nWhich occupation do you want to " << command << "?" << std::endl;
        printIndicesInList();

        std::cout << "\nEnter a number above to select a specific occupation." << std::endl;
        int jobIndex = stoi(promptNumber());

        if (jobsList.searchListByIndex(jobIndex)) {
            return jobIndex;
        }
        std::cout << "\nThat index does not exist in the list. Try again." << std::endl;
    } while (true);
}

void MenuHandler::handleAddList() {
    const Occupation *occupationToAdd = chooseJobToModify("add");
    if (occupationToAdd != nullptr) {
        if (placeOccupationInList(occupationToAdd)) {
            recentChangesList.push({
                nullptr, occupationToAdd->getJobIndex(), occupationToAdd->getMatrixCodeInt(), RecentState::ADDED
            });
            savedList = false;
            std::cout << "\nOccupation successfully added to list. Returning to list menu..." << std::endl;
            return;
        }
    }
    std::cout << "\nFailed to add occupation to list. Returning to list menu..." << std::endl;
}

void MenuHandler::handleRemoveList() {
    if (jobsList.getListSize() == 0) {
        std::cout << "\nThe list is empty. Returning to list menu..." << std::endl;
        return;
    }

    int indexToRemove = handleListIndexRetrieval("remove");

    const Occupation *jobRemoved = jobsList.removeByIndex(indexToRemove);

    // list stack can just store job indices because the job still exists in the main database
    if (jobRemoved != nullptr) {
        recentChangesList.push({
            nullptr, jobRemoved->getJobIndex(), jobRemoved->getMatrixCodeInt(), RecentState::REMOVED
        });
        savedList = false;
        std::cout << "\n" << jobRemoved->getOccupation() << " is removed from the list." << std::endl;

        return;
    }
    std::cout << "\nFailed to remove occupation from list. Returning to list menu..." << std::endl;
}

void MenuHandler::handleRemoveDatabase() {
    if (allJobsDatabase.getSize() == 0) {
        std::cout << "\nThe database is empty. Returning to main menu..." << std::endl;
        return;
    }

    const Occupation *occupationToRemove = chooseJobToModify("remove");
    std::unique_ptr<Occupation> occupationToRemovePointer;

    if (occupationToRemove != nullptr) {
        occupationToRemovePointer = allJobsDatabase.removeJobFromDatabase(
            occupationToRemove->getJobIndex(), occupationToRemove->getMatrixCodeInt());
    }

    // unique pointer is moved to stack for removing items in the database
    if (occupationToRemovePointer != nullptr) {
        int index = occupationToRemovePointer->getJobIndex();
        int matrixCodeInt = occupationToRemovePointer->getMatrixCodeInt();
        recentChangesDatabase.push({std::move(occupationToRemovePointer), index, matrixCodeInt, RecentState::REMOVED});
        savedDatabase = false;
        std::cout << "\nOccupation successfully removed from database. Returning to main menu..." << std::endl;
        return;
    }
    std::cout << "\nFailed to remove occupation from database. Returning to main menu..." << std::endl;
}

// sorting dialogue function to ask if user wants to see the data sorted
void MenuHandler::handleSort(const Structure dataset) {
    // copying the necessary dataset to the sorted jobs array
    switch (dataset) {
        case Structure::MAIN_ARRAY:
            copyMainArray(sortedJobsArray);
            break;
        case Structure::SEARCH_ARRAY:
            copySearchArray(sortedJobsArray);
            break;
        case Structure::LIST:
            copyList(sortedJobsArray);
            break;
        default:
            return;
    }

    bool ascending = false;
    char menuOption;
    std::string sortCategory;
    std::string sortOrder;

    std::cout << "\nDo you want to view the data sorted? (y/n)\n" << std::endl;

    switch (yesOrNoMenu()) {
        case 'y':
            std::cout << "\nHow would you like to sort the occupations?" << std::endl
                    << "A: Occupation Title" << std::endl
                    << "B: Median Annual Wage 2024" << std::endl
                    << "C: Typical Education Needed for Entry" << std::endl
                    << "D: Work Experience in a Related Occupation" << std::endl
                    << "E: Return to Main Menu\n" << std::endl;
            menuOption = menuHandling('A', 'E', false);
            if (menuOption != 'E') {
                std::string userInput;
                while (userInput != "1" && userInput != "2") {
                    std::cout << "\nEnter 1 to sort in ascending order." << std::endl
                            << "Enter 2 to sort in descending order.\n" << std::endl;
                    std::getline(std::cin, userInput);
                }
                if (userInput[0] == '1') {
                    sortOrder = "ascending";
                    ascending = true;
                } else {
                    sortOrder = "descending";
                    ascending = false;
                }
            } else {
                std::cout << "\nReturning to main menu..." << std::endl;
                return;
            }
            switch (menuOption) {
                case 'A': // user selects alphabetical sort
                    sortCategory = "Occupation Title";
                    sortJob(sortedJobsArray, ascending,
                            [](const Occupation *job) {
                                return job->getOccupation();
                            });
                    break;
                case 'B': // user selects wage sort
                    sortCategory = "Median Annual Wage 2024";
                    sortJob(sortedJobsArray, ascending,
                            [](const Occupation *job) {
                                return job->getWage();
                            });
                    break;
                case 'C': // user selects education sort
                    sortCategory = "Typical Education Needed for Entry";
                    sortJob(sortedJobsArray, ascending,
                            [](const Occupation *job) {
                                return job->getEducationScore();
                            });
                    break;
                case 'D':
                    sortCategory = "Work Experience in a Related Occupation";
                    sortJob(sortedJobsArray, ascending,
                            [](const Occupation *job) {
                                return job->getWorkExperienceScore();
                            });
                    break;
                default: // break to main menu
                    std::cout << "\nReturning to menu..." << std::endl;
                    return;
            }
            break;
        default:
            std::cout << "\nReturning to menu..." << std::endl;
            return;
    }

    printSearchSortResults(sortedJobsArray);

    std::cout << "\nData successfully sorted based on " << sortCategory << " in " << sortOrder << " order." <<
            std::endl;
    std::cout << "\nCheck output file for full table. Returning to main menu..." << std::endl;
}

void MenuHandler::handleSearch(const Structure dataset) {
    if (dataset == Structure::MAIN_DATABASE && allJobsDatabase.getSize() == 0) {
        std::cout << "\nThe database is empty. Returning to main menu..." << std::endl;
        return;
    }
    if (dataset == Structure::LIST && jobsList.getListSize() == 0) {
        std::cout << "\nThe list is empty. Returning to list menu..." << std::endl;
        return;
    }

    int lowerLimit;
    int upperLimit;
    std::string searchValue;
    const Occupation *jobToSearch;
    char input = promptOptionToSearch(dataset);

    switch (input) {
        case 'A': // search job title
            std::cout << "\nEnter the job title you want to search for:\n" << std::endl;
            std::getline(std::cin, searchValue);

            // STACK HAS ITS OWN SEARCH AND PRINT FUNCTION
            if (dataset == Structure::LIST) {
                jobsList.searchListByJob(searchValue, searchedJobsArray);
            } else if (dataset == Structure::MAIN_DATABASE) {
                allJobsDatabase.searchArrayByJob(searchedJobsArray, searchValue);
            }

            if (searchedJobsArray.getCurrentSize() > 0) {
                printSearchSortResults(searchedJobsArray);
                handleSort(Structure::SEARCH_ARRAY);
            } else {
                std::cout << "\nNo occupations found with that title." << std::endl;
            }
            break;

        case 'B': // search wage
            std::cout << "\nEnter lower limit for median annual wage to search:\n" << std::endl;
            lowerLimit = stoi(promptNumber());
            std::cout << "\nEnter upper limit for median annual wage to search:\n" << std::endl;
            upperLimit = stoi(promptNumber());

            if (dataset == Structure::LIST) {
                jobsList.searchListByWage(lowerLimit, upperLimit, searchedJobsArray);
            } else {
                allJobsDatabase.searchArrayByWage(searchedJobsArray, lowerLimit, upperLimit);
            }

            if (searchedJobsArray.getCurrentSize() > 0) {
                printSearchSortResults(searchedJobsArray);
                handleSort(Structure::SEARCH_ARRAY);
            } else {
                std::cout << "\nNo occupations found with the specified wage range." << std::endl;
            }
            break;

        case 'C': // search matrix code / exit
            if (dataset == Structure::LIST) {
                std::cout << "\nReturning to list menu..." << std::endl;
                return;
            }
            jobToSearch = buildKeyAndSearch();
            // prints out a single job
            printTableHeadings();
            printTableEntry(jobToSearch);
            break;

        default:
            std::cout << "\nReturning to main menu..." << std::endl;
    }
}

void MenuHandler::handleCompare() {
    if (allJobsDatabase.getSize() == 0) {
        std::cout << "\nThe database is empty. Returning to main menu..." << std::endl;
        return;
    }

    std::cout << "\nHow many occupations do you want to compare?" << std::endl;

    int numberOfJobs;

    do {
        std::cout << "\nSelect at least 2 occupations.\n" << std::endl;
        numberOfJobs = stoi(promptNumber());
    } while (numberOfJobs < 2 || numberOfJobs > 100);

    comparedJobsArray = DynamicArray<const Occupation *>(numberOfJobs);

    for (int i = 0; i < numberOfJobs; i++) {
        std::cout << "\nSelecting job " << (i + 1) << " out of " << numberOfJobs << "." << std::endl;

        const Occupation *jobToCompare = chooseJobToModify("compare");

        if (jobToCompare == nullptr) {
            std::cout << "\nJob not found. Do you want to return to the main menu? (y/n)\n" << std::endl;
            switch (yesOrNoMenu()) {
                case 'y':
                    std::cout << "\nReturning to main menu..." << std::endl;
                    return;
                default:
                    i--; // decrements i so that user can try searching for a job again by title
                    continue;
            }
        }
        comparedJobsArray.append(jobToCompare);
    }
    printComparisonResults();
}

void MenuHandler::handleUndoDatabase() {

    try {
        auto top = recentChangesDatabase.pop();
        int index = top.jobIndex;
        std::string jobTitle;

        if (top.recentState == RecentState::ADDED) {
            int matrixCodeInt = top.matrixCodeInt;

            std::unique_ptr<Occupation> jobToUndo = allJobsDatabase.removeJobFromDatabase(index, matrixCodeInt);
            jobTitle = jobToUndo->getOccupation();
            savedDatabase = false;

            std::cout << "\n'" << jobTitle << "' is removed from the database." << std::endl;
        } else if (top.recentState == RecentState::REMOVED) {
            jobTitle = top.job->getOccupation();
            allJobsDatabase.restoreJob(std::move(top.job), index);
            std::cout << "\n'" << jobTitle << "' is restored to the database." << std::endl;
        }
    }
    // catch statement to handle exceptions when stack is empty
    catch (const std::out_of_range &e) {
        std::cout << "\n" << e.what() << std::endl;
    }
}

void MenuHandler::handleUndoList() {
    try {
        auto top = recentChangesList.pop();

        if (top.recentState == RecentState::ADDED) {
            int index = top.jobIndex;

            auto job = jobsList.removeByIndex(index);

            if (job == nullptr) {
                throw std::runtime_error("The job no longer exists in the main array");
            }

            std::string jobTitle = job->getOccupation();
            savedList = false;

            std::cout << "\n'" << jobTitle << "' is removed from the list." << std::endl;
        } else if (top.recentState == RecentState::REMOVED) {
            const Occupation *job = allJobsDatabase.searchJobByCode(top.matrixCodeInt);

            if (job == nullptr) {
                throw std::runtime_error("The job no longer exists in the main array");
            }

            std::string jobTitle = job->getOccupation();
            jobsList.append(job);
            savedList = false;

            std::cout << "\n'" << jobTitle << "' is restored to the database." << std::endl;
        }
    } catch (const std::out_of_range &e) {
        std::cout << "\n" << e.what() << std::endl;
    } catch (const std::runtime_error &e) {
        std::cout << "\n" << e.what() << std::endl;
    }
}


bool MenuHandler::handleCheckSaved() {
    if (savedDatabase && savedList) {
        return true;
    }

    if (!savedDatabase && !savedList) {
        std::cout << "\nThere are unsaved changes in main database and list." << std::endl;
    }
    if (savedDatabase && !savedList) {
        std::cout << "\nThere are unsaved changes in list." << std::endl;
    }
    if (!savedDatabase && savedList) {
        std::cout << "\nThere are unsaved changes in main database." << std::endl;
    }

    std::cout << "\nDo you still want to exit the program? (y/n)" << std::endl;
    switch (yesOrNoMenu()) {
        case 'y':
            return true;
        default:
            std::cout << "\nReturning to main menu..." << std::endl;
            return false;
    }
}

void MenuHandler::handleDatabasePrint() {
    if (allJobsDatabase.getSize() == 0) {
        std::cout << "\nThe main database is empty. Returning to main menu..." << std::endl;
        return;
    }

    std::cout << "\nDo you want to view the main database or the hash table?" << std::endl
            << "A: Main Database" << std::endl
            << "B: Hash Table" << std::endl
            << "C: Return to Main Menu\n" << std::endl;
    char input = menuHandling('A', 'C', false);

    switch (input) {
        case 'A':
            printMainArray();
            handleSort(Structure::MAIN_ARRAY);
            break;
        case 'B':
            printHashTable();
            handleSort(Structure::MAIN_ARRAY);
            break;
        default:
            std::cout << "\nReturning to main menu..." << std::endl;
            break;
    }
}

// function to print the entire list to the console and output file
void MenuHandler::handleListPrint() {
    // prints message if list is empty
    if (jobsList.getListSize() == 0) {
        std::cout << "\nYour list is currently empty." << std::endl;
        return;
    }

    printTableHeadings();

    jobsList.forEachJobInList([&](const Occupation *job) {
        printTableEntry(job);
    });

    // reports the number of jobs in the list
    std::cout << "\nThere are currently " << jobsList.getListSize() << " occupations in your list." << std::endl;

    handleSort(Structure::LIST);
}

void MenuHandler::handleStackPrint(const JobStack &stack, Structure dataset) const {
    if (stack.getCurrentLength() > 0) {
        printEntireStack(stack, dataset);
    } else {
        std::cout << "\nThere are no jobs to display at the moment. Returning to main menu..." << std::endl;
    }
}

void MenuHandler::handleClearList() {
    std::cout << "\nThis will delete any existing lists and create a new one. Do you want to proceed? (y/n)\n" <<
            std::endl;
    char input = yesOrNoMenu();

    if (input == 'y') {
        jobsList.clear();
        savedList = false;
        std::cout << "\nList successfully cleared. Returning to main menu..." << std::endl;
    }
}
