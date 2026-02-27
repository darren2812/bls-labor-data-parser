#include <iostream>
#include <iomanip>
#include "MenuHandler.h"
#include "Helpers.h"


MenuHandler::MenuHandler()
    : rawData("./input/rawData.txt"), listData("./input/listData.txt"), output("./output/output.txt") {
    std::string dummy;

    // sets default table column lengths to the size of the headings + 1
    for (int i = 0; i < NUM_OF_HEADINGS; i++) {
        tableColumnLengths[i] = tableHeadings[i].size() + 1;
    }

    if (!rawData.is_open()) {
        std::cout << "\nThe file rawData.txt could not be opened" << std::endl;
        return;
    }

    // count number of lines and assign to jobInput temporarily
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

    // divide number of rows by number of rows per entry which is 16
    totalJobsCapacity = numberOfRows / NUM_OF_HEADINGS;
}

MenuHandler::~MenuHandler() {
    rawData.close();
    listData.close();
    output.close();
}

void MenuHandler::allocateDataStructures() {
    allJobsDatabase = JobDatabase(totalJobsCapacity);
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
            output << std::left << std::setw(tableColumnLengths[i]) << tableHeadings[i] << "|";
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
void MenuHandler::printTableEntry(const Occupation &currentJob) {
    // outputs each line of table to the output file
    output << std::left << std::setw(tableColumnLengths[0]) << currentJob.getOccupation() << "|"
            << std::setw(tableColumnLengths[2]) << currentJob.getOccupationType() << "|"
            << std::setw(tableColumnLengths[3]) << currentJob.getEmploymentCurrentString() << "|"
            << std::setw(tableColumnLengths[4]) << currentJob.getEmploymentFutureString() << "|"
            << std::setw(tableColumnLengths[5]) << currentJob.getDistributionCurrentString() << "|"
            << std::setw(tableColumnLengths[6]) << currentJob.getDistributionFutureString() << "|"
            << std::setw(tableColumnLengths[7]) << currentJob.getNumericChangeString() << "|"
            << std::setw(tableColumnLengths[8]) << currentJob.getPercentageChangeString() << "|"
            << std::setw(tableColumnLengths[9]) << currentJob.getPercentSelfEmployedString() << "|"
            << std::setw(tableColumnLengths[10]) << currentJob.getJobOpeningsString() << "|"
            << std::setw(tableColumnLengths[11]) << currentJob.getWageString() << "|"
            << std::setw(tableColumnLengths[12]) << currentJob.getEducation() << "|"
            << std::setw(tableColumnLengths[13]) << currentJob.getWorkExperience() << "|"
            << std::setw(tableColumnLengths[14]) << currentJob.getTraining() << "|" << std::endl;
    // outputs each line of table to the console
    std::cout << std::left << std::setw(tableColumnLengths[0]) << currentJob.getOccupation() << "|"
            << std::setw(tableColumnLengths[3]) << currentJob.getEmploymentCurrentString() << "|"
            << std::setw(tableColumnLengths[11]) << currentJob.getWageString() << "|"
            << std::setw(tableColumnLengths[12]) << currentJob.getEducation() << "|"
            << std::setw(tableColumnLengths[13]) << currentJob.getWorkExperience() << "|" << std::endl;
}

void MenuHandler::printPrefixAndCategory(const Occupation &jobCategory) const {
    std::cout << jobCategory.getMatrixPrefix()
            << ": " << jobCategory.getOccupation() << std::endl;
}

void MenuHandler::printSuffixAndJob(const Occupation &job) const {
    std::cout << job.getMatrixSuffix()
            << ": " << job.getOccupation() << std::endl;
}

void MenuHandler::printSearchedJobs() {
    int arraySize = searchedJobsArray.getCurrentSize();

    // outputs table headings
    printTableHeadings();

    // passes the print table entry function to the iterator function in the database class
    for (int i = 0; i < arraySize; i++) {
        printTableEntry(searchedJobsArray[i]);
    }

    output.close();
}

void MenuHandler::printAllCategories() const {
    allJobsDatabase.forEachCategory([&](const Occupation &jobCategory) {
        printPrefixAndCategory(jobCategory);
    });
}

void MenuHandler::printCategoryContents(const std::string &prefix) const {
    allJobsDatabase.forEachJobInCategory(prefix, [&](const Occupation &job) {
        printSuffixAndJob(job);
    });
}

void MenuHandler::printIndicesInList() const {
    jobsList.forEachJobInList([&](Occupation *&job) {
        std::cout << job->getJobIndex() << ": " << job->getOccupation() << std::endl;
    });
}

// function to print the entire list to the console and output file
void MenuHandler::printList() {
    // prints message if list is empty
    if (jobsList.getListSize() == 0) {
        std::cout << "\nYour list is currently empty." << std::endl;
        return;
    }

    printTableHeadings();

    jobsList.forEachJobInList([&](Occupation *&job) {
        printTableEntry(*job);
    });

    // reports the number of jobs in the list
    std::cout << "\nThere are currently " << jobsList.getListSize() << " occupations in your list." << std::endl;
}

// function to handle menu commands
char MenuHandler::menuHandling(char firstLetter, char lastLetter, bool acceptDash) {
    std::string userInput;
    char tempChar;

    do {
        std::getline(std::cin, userInput);
        if (acceptDash && userInput == "-") {
            return userInput[0];
        }
        tempChar = toupper(userInput[0]);
        if (tempChar < firstLetter || tempChar > lastLetter) {
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
    char tempChar = tolower(userInput[0]);

    // while loop to handle input
    while (tempChar != 'y' && tempChar != 'n') {
        std::cout << "\nInput y for YES or n for NO\n" << std::endl;
        std::getline(std::cin, userInput);
        tempChar = tolower(userInput[0]);
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

Occupation MenuHandler::promptJobAttributes(std::string jobTitle, const std::string &matrixCode) {
    // Sets occupation based on previous user input
    jobTitle[0] = toupper(jobTitle[0]);
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

    Occupation occupationToReturn = Occupation();

    occupationToReturn.setOccupation(jobTitle);
    occupationToReturn.setOccupationType(occupationType);
    occupationToReturn.setEmploymentCurrent(employmentCurrent);
    occupationToReturn.setEmploymentFuture(employmentFuture);
    occupationToReturn.setJobOpenings(jobOpenings);
    occupationToReturn.setWage(wage);
    occupationToReturn.setEducation(education);
    occupationToReturn.setWorkExperience(workExperience);
    occupationToReturn.setTraining(training);
    occupationToReturn.setMatrixCode(matrixCode);

    return occupationToReturn;
}

std::string MenuHandler::promptNumber(const std::string &messageToDisplay) {
    std::string userInput;

    do {
        std::cout << "\nEnter a number above to select " + messageToDisplay + "\n" << std::endl;
        std::getline(std::cin, userInput);

        try {
            return userInput;
        } catch (const std::invalid_argument &) {
            std::cout << "\nThe value entered is not an integer. Try again" << std::endl;
        } catch (const std::out_of_range &) {
            std::cout << "\nThe value entered is out of range. Try again" << std::endl;
        }
    } while (true);
}

void MenuHandler::handleAddDatabase() {
    std::string jobTitle;

    std::cout << "\nWhat job do you want to add?\n" << std::endl;
    std::getline(std::cin, jobTitle);
    lowerString(jobTitle);

    // outputs existing jobs to console
    if (allJobsDatabase.searchArrayByJob(searchedJobsArray, jobTitle)) {
        std::cout << std::endl;

        printSearchedJobs();

        std::cout << "\nWe have these jobs in our database. Do you still want to add an entry? (y/n)\n" << std::endl;

        switch (yesOrNoMenu()) {
            case 'y':
                break;
            case 'n':
                return;
        }
    }

    std::string matrixCode;

    do {
        printAllCategories();
        std::string prefix;

        do {
            prefix = promptNumber("a category.");
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

    Occupation jobToAdd = promptJobAttributes(jobTitle, matrixCode);

    allJobsDatabase.addJobToDatabase(jobToAdd);
    recentChangesDatabase.push({jobToAdd, "added"});

    savedDatabase = false;

    // outputs job added to the console
    std::cout << "\n'" << jobToAdd.getOccupation()
            << "' is successfully added to the database." << std::endl;
}

// function to pinpoint the specific index of an occupation
Occupation *MenuHandler::selectSpecificIndex(const std::string &command) {
    std::string userInput;
    std::getline(std::cin, userInput);
    lowerString(userInput);

    allJobsDatabase.searchArrayByJob(searchedJobsArray, userInput);

    std::cout << std::endl;

    int searchedJobsCount = searchedJobsArray.getCurrentSize();
    if (searchedJobsCount > 0) {
        for (int i = 0; i < searchedJobsCount; i++) {
            std::cout << "Index " << searchedJobsArray[i].getJobIndex() << ": "
                    << searchedJobsArray[i].getOccupation() << std::endl;
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
                if (userInput == std::to_string(searchedJobsArray[i].getJobIndex())) {
                    return &searchedJobsArray[i];
                }
            }
        }
    }

    std::cout << "Sorry, we cannot find that job in our database." << std::endl;
    return nullptr;
}

Occupation *MenuHandler::buildKeyAndSearch() {
    std::string matrixCode;
    std::string prefix;
    Occupation *jobCategory;

    do {
        printAllCategories();
        std::cout << "\nThese are the major occupation groups in the database." << std::endl;

        prefix = promptNumber("a category.");
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

        std::string suffix = promptNumber("a specific occupation.");
        if (suffix == "0") {
            suffix = "0000";
        }

        Occupation *jobToReturn = allJobsDatabase.searchJobByCode(stoi(prefix + suffix));

        if (jobToReturn != nullptr) {
            return jobToReturn;
        }
        std::cout << "\nOccupation not found. Try again." << std::endl;
    } while (true);
}

Occupation *MenuHandler::chooseJobToAdd() {
    std::cout << "\nDo you want to search for the occupation to add by title or by matrix code?" << std::endl
            << "A: Title" << std::endl
            << "B: Matrix Code" << std::endl
            << "C: Return to List Menu\n" <<
            std::endl;
    switch (menuHandling('A', 'B', false)) {
        case 'A':
            std::cout << "\nEnter the name of the occupation.\n" << std::endl;
            // function returns a pointer to the specific object in the dynamic array
            return selectSpecificIndex("add");
        case 'B':
            // buildKeyAndSearch returns nullptr if user wants to return to the main menu
            return buildKeyAndSearch();
        default:
            return nullptr;
    }
}

int MenuHandler::handleListIndexRetrieval() {
    do {
        std::cout << "\nWhich occupation do you want to add after?" << std::endl;
        printIndicesInList();

        int jobIndex = stoi(promptNumber("a specific occupation."));

        if (jobsList.searchListByIndex(jobIndex)) {
            return jobIndex;
        }
        std::cout << "\nThat index does not exist in the list. Try again." << std::endl;
    } while (true);
}

bool MenuHandler::placeOccupationInList(Occupation *occupation) {
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
            break;
        case 'C':
            // calls the insert after function
            jobsList.insertAfter(occupation, handleListIndexRetrieval());
            return true;
        default:
            return false;
    }
}

void MenuHandler::handleAddList() {
    Occupation *occupationToAdd = chooseJobToAdd();
    if (occupationToAdd != nullptr) {
        if (placeOccupationInList(occupationToAdd)) {
            recentChangesDatabase.push({*occupationToAdd, "added"});
            savedDatabase = false;
            return;
        }
    }
    std::cout << "\nFailed to add occupation to list. Returning to list menu..." << std::endl;
}
