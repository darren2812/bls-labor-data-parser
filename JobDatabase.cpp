// JobDatabase.cpp
// Darren Daniel

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Helpers.h"
#include "Occupation.h"
#include "JobDatabase.h"

void readEntries(std::ifstream& rawData, Occupation* allJobs, const std::string* headings, int* columnLengths, int& jobNumber) {
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
				 and increment jobNumber to move to the next jobNumber of the dynamic array
	*/

	// reads through the entire text file line by line
	while (std::getline(rawData, tempString)) {
		switch (columnCount) {
		case 0:
			allJobs[jobNumber].setOccupation(tempString);
			break;
		case 1:
			allJobs[jobNumber].setMatrixCode(tempString);
			break;
		case 2:
			allJobs[jobNumber].setOccupationType(tempString);
			break;
		case 3:
			allJobs[jobNumber].setEmploymentCurrent(tempString);
			break;
		case 4:
			allJobs[jobNumber].setEmploymentFuture(tempString);
			break;
		case 5:
			allJobs[jobNumber].setDistributionCurrent(tempString);
			break;
		case 6:
			allJobs[jobNumber].setDistributionFuture(tempString);
			break;
		case 7:
			allJobs[jobNumber].setNumericChange(tempString);
			break;
		case 8:
			allJobs[jobNumber].setPercentageChange(tempString);
			break;
		case 9:
			allJobs[jobNumber].setPercentSelfEmployed(tempString);
			break;
		case 10:
			allJobs[jobNumber].setJobOpenings(tempString);
			break;
		case 11:
			allJobs[jobNumber].setWage(tempString);
			break;
		case 12:
			allJobs[jobNumber].setEducation(tempString);
			break;
		case 13:
			allJobs[jobNumber].setWorkExperience(tempString);
			break;
		case 14:
			allJobs[jobNumber].setTraining(tempString);
			break;
		case 15:
			allJobs[jobNumber].setHandbookContent(tempString);
			allJobs[jobNumber].setJobIndex(jobNumber);
			columnCount = -1;
			jobNumber++;
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

void viewEntries(Occupation* allJobs, const std::string* headings, int* columnLengths, const int& jobNumber) {
	// output to text file
	std::ofstream output("../output/output.txt");
	
	// outputs table headings
	printTableHeadings(output, headings, columnLengths);
	// outputs contents of the table
	for (int i = 0; i < jobNumber; i++) {
		// pass in each instance to the function to print
		printTableEntry(output, columnLengths, allJobs[i]);
	}
	// closes output file
	output.close();
}

int searchByJob(Occupation* searchedJobs, Occupation* allJobs, const int& jobNumber, std::string jobSearched) {
	// initializing counters
	int foundSearches = 0;

	// linear search for substrings
	for (int i = 0; i < jobNumber; i++) {
		// search for jobs that do not start with the word being searched
		jobSearched[0] = tolower(jobSearched[0]);
		// Chat GPT suggested using .find() instead of making own substring search algorithm
		if (allJobs[i].getOccupation().find(jobSearched) != std::string::npos) {
			searchedJobs[foundSearches] = allJobs[i];
			foundSearches++;
		}
		// search for jobs that start with the word being searched (capitalized first letter)
		jobSearched[0] = toupper(jobSearched[0]);
		if (allJobs[i].getOccupation().find(jobSearched) != std::string::npos) {
			searchedJobs[foundSearches] = allJobs[i];
			foundSearches++;
		}
	}
	return foundSearches;
}

int searchByWage(Occupation* searchedJobs, Occupation* allJobs, const int& jobNumber, const float& lowerLimit,
	const float& upperLimit) {
	// initializing counter
	int foundSearches = 0;

	// linear search to find whether wage sits between the upper and lower bounds inclusive
	for (int i = 0; i < jobNumber; i++) {
		if (allJobs[i].getWage() >= lowerLimit && allJobs[i].getWage() <= upperLimit) {
			searchedJobs[foundSearches] = allJobs[i];
			foundSearches++;
		}
	}
	return foundSearches;
}

Occupation addEntry(Occupation*& allJobs, int& jobNumber, int& totalJobsCapacity, std::string& jobAdded, int* columnLengths, std::string key) {
	// temporary std::string and float to store user input
	std::string tempString = "";
	float tempFloat = 0.0f;

	// Code from zyBooks to increase the capacity of an array
	if (totalJobsCapacity - jobNumber < 1) {
		totalJobsCapacity++;
		Occupation* newArray = new Occupation[totalJobsCapacity];
		for (int i = 0; i < jobNumber; i++) {
			newArray[i] = allJobs[i];
		}
		delete[] allJobs;
		allJobs = newArray;
	}

	// Sets occupation based on previous user input
	jobAdded[0] = toupper(jobAdded[0]);
	allJobs[jobNumber].setOccupation(jobAdded + " *");

	allJobs[jobNumber].setMatrixCode(key);

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
	switch (tempString[0]) {
	case 'A': // assigns "Summary" occupation type
		allJobs[jobNumber].setOccupationType("Summary");
		break;
	case 'B': // assigns "Line item" occupation type
		allJobs[jobNumber].setOccupationType("Line item");
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
			allJobs[jobNumber].setEmploymentCurrent("-");
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
		allJobs[jobNumber].setEmploymentCurrent(tempString);
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
			allJobs[jobNumber].setEmploymentFuture("-");
			allJobs[jobNumber].setNumericChange("-");
			allJobs[jobNumber].setPercentageChange("-");
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
		allJobs[jobNumber].setEmploymentFuture(tempString);
		roundedString.str("");
		roundedString.clear();

		// Sets numeric change by subtracting current employment from future employment
		tempFloat = allJobs[jobNumber].getEmploymentFuture() - allJobs[jobNumber].getEmploymentCurrent();
		roundedString << std::fixed << std::setprecision(1) << tempFloat;
		tempString = roundedString.str();
		allJobs[jobNumber].setNumericChange(tempString);
		roundedString.str("");
		roundedString.clear();

		// Sets percentage change by using the percentage change formula
		tempFloat = ((allJobs[jobNumber].getEmploymentFuture() - allJobs[jobNumber].getEmploymentCurrent()) / allJobs[jobNumber].getEmploymentCurrent())
			* 100;
		roundedString << std::fixed << std::setprecision(1) << round(tempFloat * 10) / 10.0f;
		tempString = roundedString.str();
		allJobs[jobNumber].setPercentageChange(tempString);
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
		}
		else {
			std::cout << "\nEnter a non-negative value for job openings." << std::endl
				<< "Enter '-' if unknown.\n" << std::endl;
			std::getline(std::cin, tempString);
			tempFloat = toFloat(tempString);
		}
	}
	allJobs[jobNumber].setJobOpenings(tempString);

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
		}
		else {
			std::cout << "\nEnter a non-negative value for median annual wage." << std::endl
				<< "Enter '-' if unknown.\n" << std::endl;
			std::getline(std::cin, tempString);
			tempFloat = toFloat(tempString);
		}
	}
	allJobs[jobNumber].setWage(tempString);

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
		allJobs[jobNumber].setEducation("No formal educational credential");
		break;
	case 'B':
		allJobs[jobNumber].setEducation("High school diploma or equivalent");
		break;
	case 'C':
		allJobs[jobNumber].setEducation("Postsecondary nondegree award");
		break;
	case 'D':
		allJobs[jobNumber].setEducation("Some college, no degree");
		break;
	case 'E':
		allJobs[jobNumber].setEducation("Associate�s degree");
		break;
	case 'F':
		allJobs[jobNumber].setEducation("Bachelor�s degree");
		break;
	case 'G':
		allJobs[jobNumber].setEducation("Master�s degree");
		break;
	case 'H':
		allJobs[jobNumber].setEducation("Doctoral or professional degree");
		break;
	case '-':
		allJobs[jobNumber].setEducation("-");
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
		allJobs[jobNumber].setWorkExperience("None");
		break;
	case 'B':
		allJobs[jobNumber].setWorkExperience("Less than 5 years");
		break;
	case 'C':
		allJobs[jobNumber].setWorkExperience("5 years or more");
		break;
	case '-':
		allJobs[jobNumber].setWorkExperience("-");
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
		allJobs[jobNumber].setTraining("None");
		break;
	case 'B':
		allJobs[jobNumber].setTraining("Short-term on-the-job training");
		break;
	case 'C':
		allJobs[jobNumber].setTraining("Moderate-term on-the-job training");
		break;
	case 'D':
		allJobs[jobNumber].setTraining("Long-term on-the-job training");
		break;
	case 'E':
		allJobs[jobNumber].setTraining("Apprenticeship");
		break;
	case 'F':
		allJobs[jobNumber].setTraining("Internship/residency");
		break;
	case '-':
		allJobs[jobNumber].setTraining("-");
		break;
	default:
		break;
	}

	// sets the index of the newly added job
	allJobs[jobNumber].setJobIndex(jobNumber);

	// increments jobNumber
	jobNumber++;
	// returns recently added job
	return allJobs[jobNumber - 1];
}

void addEntryAgain(Occupation jobAdded, Occupation*& allJobs, int& jobNumber) {
	int indexAdded = jobAdded.getJobIndex();
	// add entry shifts elements to the right
	for (int i = jobNumber - 1; i >= indexAdded; i--) {
		// starts from the end of the array and shifts elements to the right (suggested by ChatGPT)
		allJobs[i].setJobIndex(allJobs[i].getJobIndex() + 1);
		allJobs[i + 1] = allJobs[i];
	}
	allJobs[indexAdded] = jobAdded;
	// increments jobNumber to represent increasing number of jobs in the database
	jobNumber++;
}

Occupation removeEntry(Occupation*& allJobs, int& jobNumber, int& totalJobsCapacity, int indexRemoved) {
	Occupation jobToReturn = allJobs[indexRemoved];
	// remove entry shifts elements to the left instead of resizing
	for (int i = indexRemoved + 1; i < jobNumber; i++) {	
		// decrements jobIndex datafield of each job to reflect shifting elements to the left
		allJobs[i].setJobIndex(allJobs[i].getJobIndex() - 1);
		allJobs[i - 1] = allJobs[i];
	}
	// decrements jobNumber to represent decreasing number of jobs in the database
	jobNumber--;
	// returns occupation being removed
	return jobToReturn;
}

void rewriteJobFile(Occupation* allJobs, const int& jobNumber) {
	// opens the input file as an output file stream and automatically wipes the file's contents
	std::ofstream modifiedData("../input/rawData.txt");
	// iterates through entire dynamic array
	for (int i = 0; i < jobNumber; i++) {
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

void importList(std::ifstream& listData, Occupation* allJobs, SinglyLinkedList* list, int jobNumber, HashTable& hashTable) {
	// setting the cursor position back to the start
	listData.clear();
	listData.seekg(0, std::ios::beg);
	std::string initialString;
	std::string tempString;
	int matrixCode;
	Occupation* jobPointer;
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
			}
			else {
				std::cout << "\nFailed to find occupation with matrix code " << initialString << std::endl;
			}
		}
		catch (...) {
			std::cout << "\nSkipping " << initialString << " from list input file." << std::endl;
		}
	}
}

void rewriteListFile(SinglyLinkedList* list) {
	// opens the input file as an output file stream and automatically wipes the file's contents
	std::ofstream modifiedData("../input/listData.txt");
	SinglyLinkedNode<Occupation>* current = list->getListHead();
	// iterates through the list and outputs the matrix codes
	while (current) {
		modifiedData << current->data.getMatrixCode() << std::endl;
		current = current->next;
	}
	// closes file
	modifiedData.close();
}
