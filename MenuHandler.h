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
    DynamicArray<const Occupation*> searchedJobsArray;
    DynamicArray<const Occupation*> sortedJobsArray;
    DynamicArray<const Occupation*> comparedJobsArray;
    SinglyLinkedList jobsList;
    JobStack recentChangesDatabase;
    JobStack recentChangesList;

    size_t totalJobsCapacity;
    bool savedDatabase = false;
    bool savedList = false;

    // ENUM for handling different sorting data structures
    enum class Structure {
        MAIN_DATABASE,
        MAIN_ARRAY,
        SEARCH_ARRAY,
        LIST
    };

public:
    MenuHandler();
    ~MenuHandler();

    void allocateDatabase();
    void displayMainMenu();
    void displayListMenu();
    void run();
    void runListMenu();

    void printTableHeadings();
    void printTableEntry(const Occupation *currentJob);

    void printPrefixAndCategory(const Occupation &jobCategory) const;
    void printSuffixAndJob(const Occupation &job) const;

    void printMainArray();
    void printHashTable();
    void printSearchSortResults(DynamicArray<const Occupation *> &array);
    void printEntireStack(JobStack &stack, Structure dataset) const;
    void printBarChart(float maxValue, float maxChartLength, const std::function<float(const Occupation *)> &fn);
    void printComparisonResults();
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
    std::string promptNumber() const;
    char promptOptionToSearch(Structure dataset);

    // function to select a specific index from the database
    const Occupation* selectSpecificIndex(const std::string &command);
    // helper function to search for a particular job in the hash table
    const Occupation* buildKeyAndSearch() const;
    const Occupation* chooseJobToModify(const std::string &command);
    bool placeOccupationInList(const Occupation* occupation);

    bool databasedIsSaved();
    bool listIsSaved();

    // functions to copy the contents of one array to another
    void copyMainArray(DynamicArray<const Occupation *> &outputArray);
    void copySearchArray(DynamicArray<const Occupation *> &outputArray);
    void copyList(DynamicArray<const Occupation *> &outputArray);

    void handleAddDatabase();
    int handleListIndexRetrieval();
    void handleAddList();
    void handleRemoveList();
    void handleRemoveDatabase();
    void handleSort(DynamicArray<const Occupation *> &sortedJobs, Structure dataset);
    void handleSearch(Structure dataset);
    void handleCompare();
    void handleUndoDatabase();
    void handleUndoList();
    bool handleCheckSaved();
    void handleDatabasePrint();
    void handleStackPrintAndSearch(Structure dataset);
    void handleClearList();
};
