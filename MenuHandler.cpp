#include "MenuHandler.h"

MenuHandler::MenuHandler()
    : rawData("./input/rawData.txt"), listData("./input/listData.txt") {
    std::string dummy;

    // sets default table column lengths to the size of the headings + 1
    for (int i = 0; i < NUM_OF_HEADINGS; i++) {
        columnLengths[i] = headings[i].size() + 1;
    }

    // code from zybooks to read file
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

void MenuHandler::allocateDataStructures() {
    allJobsDatabase = JobDatabase(totalJobsCapacity);
}

// function to print headings
void MenuHandler::printTableHeadings(std::ofstream &output, const std::string *headings, int *columnLengths) const {
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
            output << std::left << std::setw(columnLengths[i]) << headings[i] << "|";
            totalWidth += columnLengths[i];
            // takes widths from job title, employment, wage, education, and work experience columns for console summary
            if (i == 0 || i == 3 || i == 11 || i == 12 || i == 13) {
                std::cout << std::left << std::setw(columnLengths[i]) << headings[i] << "|";
                consoleWidth += columnLengths[i];
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
void MenuHandler::printTableEntry(std::ofstream &output, const int *columnLengths, const Occupation &currentJob) const {
    // outputs each line of table to the output file
    output << std::left << std::setw(columnLengths[0]) << currentJob.getOccupation() << "|"
            << std::setw(columnLengths[2]) << currentJob.getOccupationType() << "|"
            << std::setw(columnLengths[3]) << currentJob.getEmploymentCurrentString() << "|"
            << std::setw(columnLengths[4]) << currentJob.getEmploymentFutureString() << "|"
            << std::setw(columnLengths[5]) << currentJob.getDistributionCurrentString() << "|"
            << std::setw(columnLengths[6]) << currentJob.getDistributionFutureString() << "|"
            << std::setw(columnLengths[7]) << currentJob.getNumericChangeString() << "|"
            << std::setw(columnLengths[8]) << currentJob.getPercentageChangeString() << "|"
            << std::setw(columnLengths[9]) << currentJob.getPercentSelfEmployedString() << "|"
            << std::setw(columnLengths[10]) << currentJob.getJobOpeningsString() << "|"
            << std::setw(columnLengths[11]) << currentJob.getWageString() << "|"
            << std::setw(columnLengths[12]) << currentJob.getEducation() << "|"
            << std::setw(columnLengths[13]) << currentJob.getWorkExperience() << "|"
            << std::setw(columnLengths[14]) << currentJob.getTraining() << "|" << std::endl;
    // outputs each line of table to the console
    std::cout << std::left << std::setw(columnLengths[0]) << currentJob.getOccupation() << "|"
            << std::setw(columnLengths[3]) << currentJob.getEmploymentCurrentString() << "|"
            << std::setw(columnLengths[11]) << currentJob.getWageString() << "|"
            << std::setw(columnLengths[12]) << currentJob.getEducation() << "|"
            << std::setw(columnLengths[13]) << currentJob.getWorkExperience() << "|" << std::endl;
}

void MenuHandler::printPrefixAndCategory(const Occupation &jobCategory) const {
    std::cout << jobCategory.getMatrixCode().substr(0, 2) << ": " <<
            jobCategory.getOccupation() << std::endl;
}

void MenuHandler::printSearchedJobs(const JobDatabase &database, const std::string *headings,
                                    int *columnLengths) const {
    // output to text file
    std::ofstream output("../output/output.txt");
    // outputs table headings
    printTableHeadings(output, headings, columnLengths);

    // passes the print table entry function to the iterator function in the database class
    allJobsDatabase.forEachSearchedJob([&](const Occupation &currentJob) {
        printTableEntry(output, columnLengths, currentJob);
    });

    output.close();
}

void MenuHandler::printAllCategories(const JobDatabase &database) const {
    allJobsDatabase.forEachCategory([&](const Occupation &jobCategory) {
        printPrefixAndCategory(jobCategory);
    });
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
        std::cout << "\nEnter a non-negative value for employment projection." << std::endl
                << "Enter '-' if unknown.\n" << std::endl;
    } while (true);

    return valueToReturn;
}

Occupation MenuHandler::promptJobAttributes(std::string jobTitle, const std::string& matrixCode) {
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

int MenuHandler::promptMatrixCodePrefix() {
    std::string userInput;

    do {
        std::cout << "\nEnter a number above to select an existing category for the new occupation.\n" << std::endl;
        std::getline(std::cin, userInput);

        try {
            return stoi(userInput);
        } catch (const std::invalid_argument &) {
            std::cout << "\nThe value entered is not an integer. Try again" << std::endl;
        } catch (const std::out_of_range &) {
            std::cout << "\nThe value entered is out of range. Try again" << std::endl;
        }
    } while (true);
}

void MenuHandler::handleAddJob() {
    std::string jobTitle;

    std::cout << "\nWhat job do you want to add?\n" << std::endl;
    std::getline(std::cin, jobTitle);
    lowerString(jobTitle);

    // outputs existing jobs to console
    if (allJobsDatabase.searchByJob(jobTitle)) {
        std::cout << std::endl;

        printSearchedJobs(allJobsDatabase, headings, columnLengths);

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
        printAllCategories(allJobsDatabase);
        int prefix;

        do {
            prefix = promptMatrixCodePrefix();

            if (allJobsDatabase.categoryExists(prefix)) {
                break;
            } else {
                std::cout << "\nThe category does not exist. Please choose a different category." << std::endl;
            }

        } while (true);

        if (allJobsDatabase.generateUniqueKey(prefix, matrixCode)) {
            break;
        }

        std::cout << "\nThis category already contains the maximum number of jobs."
                << " Please choose a different category." << std::endl;
    } while (true);

    Occupation jobToAdd = promptJobAttributes(jobTitle, matrixCode);

    allJobsDatabase.addJobToDatabase(jobToAdd, matrixCode);
    recentChangesDatabase.push({jobToAdd, "added"});

    savedDatabase = false;

    // outputs job added to the console
    std::cout << "\n'" << jobToAdd.getOccupation()
            << "' is successfully added to the database." << std::endl;
}
