#pragma once

#include <string>
#include <fstream>
#include "JobDatabase.h"
#include "LinkedList.h"
#include "Sort.h"
#include "Stack.h"

class MenuHandler {
private:
    // integer to store the number of table headings
    static constexpr int NUM_OF_HEADINGS = 16;
    // float to store the maximum length of a bar chart when comparing jobs
    float MAX_BAR_CHART_LENGTH = 100;
    // headings in indexes 1 and 15 are null to skip columns 2 and 16 in data file
    const std::string tableHeadings[NUM_OF_HEADINGS] = {
        "Occupation", "SOC Matrix Code", "Type", "Employment 2023", "Employment 2033",
        "% Distribution 2023", "% Distribution 2033", "Change 2023-33",
        "% Change 2023-33", "% Self Employed", "Annual Openings 2023-33",
        "Median Wage 2024", "Typical Education Needed", "Related Work Experience",
        "Typical On-the-Job Training", ""
    };
    // array to assign different lengths for each column and setting everything to 0
    int tableColumnLengths[NUM_OF_HEADINGS] = {};

    // file stream for data
    std::fstream rawData;
    std::fstream listData;
    std::ofstream output;

    // DATA STRUCTURES
    JobDatabase allJobsDatabase;
    DynamicArray<Occupation*> searchedJobsArray;
    DynamicArray<Occupation*> sortedJobsArray;
    // declaring linked list pointer suggested by chatGPT
    SinglyLinkedList jobsList;
    // creating stacks to track recent changes
    JobStack recentChangesDatabase;
    JobStack recentChangesList;

    // jobCounter counter in main to store the jobCounter of the last job
    int jobCounter = 0;
    // searchRows represents the number of entries successfully searched
    int searchRows = 0;
    // number of total rows in original text file
    int numberOfRows = 0;
    // max number of jobs analyzed
    int totalJobsCapacity = 0;
    // int to store matrix code when searching
    int searchCode;

    // occupation pointer to store occupation being searched
    Occupation *jobSearchedPtr = nullptr;
    // occupation object to store occupation being modified
    Occupation jobModified;
    // Job being undone
    Occupation undoneJob;
    JobPair undoneJobPair;

    // bool to determine whether any searches were found
    bool searchOutcome = false;
    // bool to determine whether user has saved or not
    bool savedDatabase = false;
    bool savedList = false;

    // VARIABLES FOR COMPARISON
    // int to store how many jobs to compare
    short jobsToCompare = 0;
    // int to act as a counter when selecting jobs
    short selectionCounter = 0;
    // float to see the largest number in the comparison
    float maxValue = 0;
    // float to keep track of the value of each hashtag
    float valueOfHashTag = 0;
    // int to keep track of the number of hashtags to print to the console
    int numberOfHashTags = 0;
    // lambda to pass into functions when printing bar charts
    float getterLambda;

public:
    MenuHandler();
    ~MenuHandler();

    void allocateDataStructures();

    // table related functions
    void printTableHeadings();

    void printTableEntry(const Occupation &currentJob);

    void printPrefixAndCategory(const Occupation &jobCategory) const;
    void printSuffixAndJob(const Occupation &job) const;

    void printSearchedJobs();

    void printAllCategories() const;
    void printCategoryContents(const std::string &prefix) const;
    void printIndicesInList() const;
    void printList();

    // function to handle menu selection for reusability
    char menuHandling(char firstLetter, char lastLetter, bool acceptDash);
    // function to handle yes/no selection for reusability
    char yesOrNoMenu();

    // job adding prompts
    OccupationRow promptJobAttributes(std::string jobTitle, const std::string &matrixCode);

    std::string promptNonNegativeOrDash();
    std::string promptNumber(const std::string &messageToDisplay);

    void handleAddDatabase();

    // function to select a specific index from the database
    Occupation* selectSpecificIndex(const std::string &command);
    // helper function to search for a particular job in the hash table
    Occupation* buildKeyAndSearch();
    Occupation* chooseJobToAdd();
    bool placeOccupationInList(Occupation* occupation);
    int handleListIndexRetrieval();
    void handleAddList();
};
