#pragma once

#include <string>
#include <fstream>
#include "JobDatabase.h"
#include "LinkedList.h"
#include "Sort.h"
#include "Stack.h"

class MenuHandler {
private:
    static constexpr int NUM_OF_HEADINGS = 16;
    float MAX_BAR_CHART_LENGTH = 100;
    const std::string tableHeadings[NUM_OF_HEADINGS] = {
        "Occupation", "SOC Matrix Code", "Type", "Employment 2023", "Employment 2033",
        "% Distribution 2023", "% Distribution 2033", "Change 2023-33",
        "% Change 2023-33", "% Self Employed", "Annual Openings 2023-33",
        "Median Wage 2024", "Typical Education Needed", "Related Work Experience",
        "Typical On-the-Job Training", ""
    };
    // array to assign different lengths for each column and setting everything to 0
    size_t tableColumnLengths[NUM_OF_HEADINGS] = {};

    std::string rawDataPath;
    std::string listDataPath;
    std::string outputPath;
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
    bool savedDatabase = true;
    bool savedList = true;

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
    static void displayMainMenu();
    static void displayListMenu();
    void run();
    void runListMenu();

    void printTableHeadings();
    void printTableEntry(const Occupation *currentJob);

    static void printPrefixAndCategory(const Occupation &jobCategory);
    static void printSuffixAndJob(const Occupation &job);

    void printMainArray();
    void printHashTable();
    void printSearchSortResults(DynamicArray<const Occupation *> &array);
    void printEntireStack(const JobStack &stack, Structure dataset) const;
    void printBarChart(float maxValue, float maxChartLength, const std::function<float(const Occupation *)> &fn);
    void printComparisonResults();
    void printAllCategories() const;
    void printCategoryContents(const std::string &prefix) const;
    void printIndicesInList() const;
    static void printSingleJob(const Occupation* job);

    // function to handle menu selection
    static unsigned char menuHandling(char firstLetter, char lastLetter, bool acceptDash);
    // function to handle yes/no selection
    static unsigned char yesOrNoMenu();

    static OccupationRow promptJobAttributes(std::string jobTitle, const std::string &matrixCode);

    static std::string promptNonNegativeOrDash();
    static std::string promptNumber();
    static unsigned char promptOptionToSearch(Structure dataset);

    // function to select a specific index from the database
    const Occupation* selectSpecificIndex(const std::string &command);
    // helper function to search for a particular job in the hash table
    const Occupation* buildKeyAndSearch() const;
    const Occupation* chooseJobToModify(const std::string &command);
    bool placeOccupationInList(const Occupation* occupation);

    bool databasedIsSaved() const;
    bool listIsSaved() const;

    // functions to copy the contents of one array to another
    void copyMainArray(DynamicArray<const Occupation *> &outputArray) const;
    void copySearchArray(DynamicArray<const Occupation *> &outputArray) const;
    void copyList(DynamicArray<const Occupation *> &outputArray) const;

    void handleAddDatabase();
    int handleListIndexRetrieval(const std::string &command) const;
    void handleAddList();
    void handleRemoveList();
    void handleRemoveDatabase();
    void handleSort(Structure dataset);
    void handleSearch(Structure dataset);
    void handleCompare();
    void handleUndoDatabase();
    void handleUndoList();
    bool handleCheckSaved() const;
    void handleDatabasePrint();
    void handleListPrint();
    void handleStackPrint(const JobStack &stack, Structure dataset) const;
    void handleClearList();
};
