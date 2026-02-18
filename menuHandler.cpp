#include "menuHandler.h"

MenuHandler::MenuHandler()
: rawData("./input/rawData.txt"), listData("./input/listData.txt")  {

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
        std::cout << "\nThe number of rows in the input file are incompatible. Try redownloading the input source file.\n" << std::endl
            << "Exiting the program..." << std::endl;
        return;
    }

    // divide number of rows by number of rows per entry which is 16
    totalJobsCapacity = numberOfRows / NUM_OF_HEADINGS;
}

void MenuHandler::initializeDataStructures() {

}
