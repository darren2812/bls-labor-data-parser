// Sort.cpp
// Darren Daniel

#include <iostream>
#include "Sort.h"
#include "Helpers.h"
#include "OldDatabase.h"

// getter functions for mergeSort
std::string getThisOccupation(const Occupation& job) {
	return job.getOccupation();
}
float getThisWage(const Occupation& job) {
	return job.getWage();
}
int getThisEducationScore(const Occupation& job) {
	return job.getEducationScore();
}
int getThisWorkExperienceScore(const Occupation& job) {
	return job.getWorkExperienceScore();
}


// sorting dialogue function to ask if user wants to see the data sorted
void sortingDialogue(Occupation* allJobs, Occupation* sortedJobs, int jobCounter, const std::string* headings, int* columnLengths) {
	std::string userInput = "-";
	bool ascending = false;
	char menuOption;
	std::cout << "\nDo you want to view the data sorted? (y/n)\n" << std::endl;
	std::string sortCategory = "-";
	std::string sortOrder = "-";
	
	switch (yesOrNoMenu()) {
	case 'y':
		std::cout << "\nHow would you like to sort the occupations?" << std::endl
			<< "A: Occupation Title" << std::endl
			<< "B: Median Annual Wage 2024" << std::endl
			<< "C: Typical Education Needed for Entry" << std::endl
			<< "D: Work Experience in a Related Occupation" << std::endl
			<< "E: Return to Main Menu\n" << std::endl;
		menuOption = menuHandling('A', 'E', userInput);
		if (menuOption != 'E') {
			while (userInput != "1" && userInput != "2") {
				std::cout << "\nEnter 1 to sort in ascending order." << std::endl
					<< "Enter 2 to sort in descending order.\n" << std::endl;
				std::getline(std::cin, userInput);
			}
			switch (userInput[0]) {
			case '1':
				sortOrder = "ascending";
				ascending = true;
				break;
			case '2':
				sortOrder = "descending";
				ascending = false;
				break;
			}
		}
		switch (menuOption) {
		case 'A': // user selects alphabetical sort
			sortCategory = "Occupation Title";
			sortJob(allJobs, sortedJobs, jobCounter, ascending, getThisOccupation);
			break;
		case 'B': // user selects wage sort
			sortCategory = "Median Annual Wage 2024";
			sortJob(allJobs, sortedJobs, jobCounter, ascending, getThisWage);
			break;
		case 'C': // user selects education sort
			sortCategory = "Typical Education Needed for Entry";
			sortJob(allJobs, sortedJobs, jobCounter, ascending, getThisEducationScore);
			break;
		case 'D':
			sortCategory = "Work Experience in a Related Occupation";
			sortJob(allJobs, sortedJobs, jobCounter, ascending, getThisWorkExperienceScore);
			break;
		default: // break to main menu
			return;
		}
		break;
	case 'n':
		return;
	default:
		return;
	}
	viewEntries(sortedJobs, headings, columnLengths, jobCounter);
	std::cout << "\nData successfully sorted based on " << sortCategory << " in " << sortOrder << " order." << std::endl;
	std::cout << "\nCheck output file for full table." << std::endl;
}