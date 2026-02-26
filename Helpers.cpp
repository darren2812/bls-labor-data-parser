// Helpers.cpp
// Darren Daniel

#include <iostream>
#include <iomanip>
#include <chrono>
#include "LinkedList.h"
#include "Helpers.h"
#include "OldDatabase.h"
#include "Occupation.h"

// initializing global catch variable to determine if catch statement is called
bool g_catch = false;

// ChatGPT suggested using a try and catch statement to handle missing values (i.e. "-")
float toFloat(std::string& s) {
	try {
		g_catch = false;
		return stof(s);
	}
	catch (...) {
		g_catch = true;
		return 0.0f;
	}
}

// function to make std::string lowercase
void lowerString(std::string& input) {
	for (int i = 0; i < input.length(); i++) {
		input[i] = tolower(input[i]);
	}
}

// function to pinpoint the specific index of an occupation
Occupation* selectSpecficIndex(Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter, std::string& userInput, std::string command) {
	std::getline(std::cin, userInput);
	lowerString(userInput);
	int searchRows = searchByJob(searchedJobs, allJobs, jobCounter, userInput);
	std::cout << std::endl;
	if (searchRows > 0) {
		for (int i = 0; i < searchRows; i++) {
			std::cout << "Index " << searchedJobs[i].getJobIndex() << ": "
				<< searchedJobs[i].getOccupation() << std::endl;
		}
		// chatGPT suggested using while(true) instead of using an additional boolean
		while (true) {
			std::cout << "\nEnter the index (number) that you want to " << command << ":" << std::endl
				<< "If you want to return to the main menu, enter 'menu'.\n" << std::endl;
			std::getline(std::cin, userInput);
			lowerString(userInput);
			if (userInput == "menu") {
				return nullptr;
			}
			for (int i = 0; i < searchRows; i++) {
				if (userInput == std::to_string(searchedJobs[i].getJobIndex())) {
					// return address of job from original array (solution by ChatGPT)
					return &allJobs[searchedJobs[i].getJobIndex()];

				}
			}
		}
	}
	else {
		std::cout << "Sorry, we cannot find that job in our database." << std::endl;
	}
	return nullptr;
}

// helper function to handle all search functions
bool searchFunction(SinglyLinkedList* list, std::string dataStructure, std::string searchType, Occupation* searchedJobs, Occupation* allJobs, const int& jobCounter,
	const std::string* headings, int* columnLengths, int& searchRows) {

	std::string input = "-";
	// lower and upper limits for wage search
	float lowerLimit = 0.0f;
	float upperLimit = 0.0f;
	// time counters, chrono implementation taken from cpp reference website and ChatGPT
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point endTime;
	std::chrono::microseconds timeTaken;

	if (searchType == "title") {
		std::cout << "\nWhat job do you want to search for?\n" << std::endl;
		std::getline(std::cin, input);
		lowerString(input);

		// start of search algorithm and outputs time taken in microseconds
		startTime = std::chrono::high_resolution_clock::now();
		// searchRows based on what function is being passed in as an argument
		if (dataStructure == "array") {
			searchRows = searchByJob(searchedJobs, allJobs, jobCounter, input);
		}
		else if (dataStructure == "list") {
			searchRows = list->searchListByJob(input, searchedJobs);
		}
		endTime = std::chrono::high_resolution_clock::now();
		timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

		// displays entries found in output file
		if (searchRows == 0) {
			// prints time to search regardless of whether entries were found
			std::cout << "\nTime to search: " << timeTaken.count() << " microseconds." << std::endl;
			// message if no jobs are found
			std::cout << "\nSorry, we cannot find that job in our database." << std::endl;
			return false;
		}
		else {
			viewEntries(searchedJobs, headings, columnLengths, searchRows);
			// prints time to search regardless of whether entries were found
			std::cout << "\nTime to search: " << timeTaken.count() << " microseconds." << std::endl;
			std::cout << "\nCheck output file for full table." << std::endl;
			return true;
		}
	}

	else if (searchType == "wage") {
		std::cout << "\nEnter lower limit for median annual wage to search:\n" << std::endl;
		std::getline(std::cin, input);
		lowerLimit = toFloat(input);
		// any std::string makes g_catch true in toFloat()
		while (g_catch == true) {
			std::cout << "\nEnter a numeric value for the lower limit:\n" << std::endl;
			std::getline(std::cin, input);
			lowerLimit = toFloat(input);
		}

		// asks user for upper limit and assigns that value
		std::cout << "\nEnter upper limit for median annual wage to search:\n" << std::endl;
		std::getline(std::cin, input);
		upperLimit = toFloat(input);
		while (g_catch == true) {
			std::cout << "\nEnter a numeric value for the upper limit:\n" << std::endl;
			std::getline(std::cin, input);
			upperLimit = toFloat(input);
		}

		// start of search algorithm and outputs time taken in microseconds
		startTime = std::chrono::high_resolution_clock::now();
		// searchRows based on what function is entered
		if (dataStructure == "array") {
			searchRows = searchByWage(searchedJobs, allJobs, jobCounter, lowerLimit, upperLimit);
		}
		else if (dataStructure == "list") {
			searchRows = list->searchListByWage(lowerLimit, upperLimit, searchedJobs);
		}
		endTime = std::chrono::high_resolution_clock::now();
		timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

		// message if no jobs are found
		if (searchRows == 0) {
			// prints time to search regardless of whether entries were found
			std::cout << "\nTime to search: " << timeTaken.count() << " microseconds." << std::endl;
			std::cout << "\nSorry, we cannot find jobs within the wage range you specified." << std::endl;
			return false;
		}
		else {
			// displays searched entries in output file
			viewEntries(searchedJobs, headings, columnLengths, searchRows);
			// prints time to search regardless of whether entries were found
			std::cout << "\nTime to search: " << timeTaken.count() << " microseconds." << std::endl;
			std::cout << "\nWe found " << searchRows << " matching jobs in our database. Check output file for full table." << std::endl;
			return true;
		}
	}

	else {
		std::cout << "\nNo search functions were called." << std::endl;
		return false;
	}
}

Occupation* buildKeyAndSearch(Occupation* allJobs, int jobCounter, HashTable& hashTable) {
	std::string firstHalfKey = "00";
	std::string userInput;
	Occupation* jobSearched = nullptr;
	int searchCode;

	// displays the major occupation groups in the table (https://www.bls.gov/oes/2023/may/oes_stru.htm)
	std::cout << std::endl;
	for (int i = 0; i < jobCounter; i++) {
		// major groups are denoted by four 0s at the end of the code, hence a modulo of 10000 will result in 0
		if (allJobs[i].getMatrixCodeInt() % 10000 == 0) {
			std::cout << allJobs[i].getMatrixCode().substr(0, 2) << ": " <<
				allJobs[i].getOccupation() << std::endl;
		}
	}

	std::cout << "\nThese are the major occupation groups in the database." << std::endl;

	// while loop to handle input
	while (true) {
		std::cout << "\nEnter a number above to search a job in a category." << std::endl
			<< "If you want to return to the main menu, enter 'menu'.\n" << std::endl;
		std::getline(std::cin, userInput);
		lowerString(userInput);
		if (userInput == "menu") {
			return nullptr;
		}
		else {
			try {
				// length check, to make sure that value is contained within an int
				if (userInput.length() < 5) {
					// multiplication by 10000 to search for specific category in the hash table
					searchCode = stoi(userInput) * 10000;
					jobSearched = hashTable.getJobPointer(searchCode);
					if (jobSearched != nullptr) {
						break;
					}
					else {
						std::cout << "\nMajor occupation group not found. Try again." << std::endl;
					}
				}
				else {
					std::cout << "\nInput is too long. Try again." << std::endl;
				}
			}
			catch (std::invalid_argument) {
				std::cout << "\nThe value entered is not an integer. Try again" << std::endl;
			}
		}
	}
	// assigning jobInput with userInput to construct first half of the key
	if (stoi(userInput) != 0) {
		firstHalfKey = userInput;
	}
	std::cout << std::endl;
	for (int i = 0; i < jobCounter; i++) {
		// displaying specific jobs now
		if (allJobs[i].getMatrixCode().substr(0, 2) == firstHalfKey) {
			std::cout << allJobs[i].getMatrixCode().substr(3, 4) << ": " <<
				allJobs[i].getOccupation() << std::endl;
		}
	}
	std::cout << "\nThese are the specific jobs under the " << jobSearched->getOccupation() << " category." << std::endl;

	// second while loop to handle input
	while (true) {
		std::cout << "\nEnter a number above to search a job in a category." << std::endl
			<< "If you want to return to the main menu, enter 'menu'.\n" << std::endl;
		std::getline(std::cin, userInput);
		lowerString(userInput);
		if (userInput == "menu") {
			return nullptr;
		}
		// length check to make sure that the input is contained within an int
		if (userInput.length() < 7) {
			// userInput becomes the second half of the key and is concatenated to firstHalfKey to make the full key
			searchCode = stoi(firstHalfKey + userInput);
			jobSearched = hashTable.getJobPointer(searchCode);
			if (jobSearched != nullptr) {
				return &allJobs[jobSearched->getJobIndex()];
			}
			else {
				std::cout << "\nMajor occupation group not found. Try again." << std::endl;
			}
		}
		else {
			std::cout << "\nInput is too long. Try again." << std::endl;
		}
	}
}

// function to mind max value in an array
float findMax(Occupation**& jobArray, int numberOfJobs, float(*function)(Occupation*&)) {
	float maxValue = 0;
	for (int i = 0; i < numberOfJobs; i++) {
		if (maxValue < function(jobArray[i])) {
			maxValue = function(jobArray[i]);
		}
	}
	return maxValue;
}

// function to print bar chart to the console
void printBarChart(Occupation**& comparedJobs, float maxValue, float maxChartLength, int jobsToCompare, float(*function)(Occupation*&)) {
	float valueOfHashTag = maxValue / maxChartLength;
	int numberOfHashTags;
	std::cout << "One # represents " << valueOfHashTag << std::endl;
	for (int j = 0; j < jobsToCompare; j++) {
		std::cout << "[" << j << "]: ";
		numberOfHashTags = function(comparedJobs[j]) / valueOfHashTag;
		for (int k = 0; k < numberOfHashTags; k++) {
			std::cout << "#";
		}
		std::cout << " (" << function(comparedJobs[j]) << ")" << std::endl;
	}
}