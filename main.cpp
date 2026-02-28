// main.cpp
// Darren Daniel

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "Helpers.h"
#include "Occupation.h"
#include "OldDatabase.h"
#include "LinkedList.h"
#include "Sort.h"
#include "Stack.h"
#include "HashTable.h"

int main() {

	// integer to store the number of table headings
	const int NUM_OF_HEADINGS = 16;
	// float to store the maximum length of a bar chart when comparing jobs
	const float MAX_BAR_CHART_LENGTH = 100;
	// file stream for data
	std::ifstream rawData("../input/rawData.txt");
	std::ifstream listData("../input/laistData.txt");
	// headings in indexes 1 and 15 are null to skip columns 2 and 16 in data file
	std::string headings[NUM_OF_HEADINGS] = { "Occupation", "SOC Matrix Code", "Type", "Employment 2023", "Employment 2033",
						   "% Distribution 2023", "% Distribution 2033", "Change 2023-33",
						   "% Change 2023-33", "% Self Employed", "Annual Openings 2023-33",
						   "Median Wage 2024", "Typical Education Needed", "Related Work Experience",
						   "Typical On-the-Job Training", "" };
	// string to take in user input
	std::string userInput = "-";
	std::string jobInput;
	std::string firstHalfKey;
	std::string secondHalfKey;

	// jobNumber counter in main to store the jobNumber of the last job
	int jobCounter = 0;
	// searchRows represents the number of entries successfully searched
	int searchRows = 0;
	// number of total rows in original text file
	int numberOfRows = 0;
	// max number of jobs analyzed
	int totalJobsCapacity = 0;
	// array to assign different lengths for each column and setting everything to 0
	int columnLengths[16] = {};
	// int to store matrix code when searching
	int searchCode;
	// occupation pointer to store occupation being searched
	Occupation* jobSearchedPtr = nullptr;
	// occupation object to store occupation being modified
	Occupation jobModified;
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

	// sets default table column lengths to the size of the headings + 1
	for (int i = 0; i < 16; i++) {
		columnLengths[i] = headings[i].size() + 1;
	}

	// code from zybooks to read file
	if (!rawData.is_open()) {
		std::cout << "\nThe file rawData.txt could not be opened" << std::endl;
		return 1;
	}

	// count number of lines and assign to jobInput temporarily
	while (std::getline(rawData, jobInput)) {
		numberOfRows++;
	}

	if (numberOfRows % 16 != 0) {
		std::cout << "\nThe number of rows in the input file are incompatible. Try redownloading the input source file.\n" << std::endl
			<< "Exiting the program..." << std::endl;
		return 1;
	}

	// divide number of rows by number of rows per entry which is 16
	totalJobsCapacity = numberOfRows / 16;

	// creating dynamic array for all jobs
	Occupation* allJobs = new Occupation[totalJobsCapacity];

	// creating hash table for all jobs
	HashTable hashTable;

	// declaring linked list pointer suggested by chatGPT
	SinglyLinkedList* list = new SinglyLinkedList;

	// creating stacks to track recent changes
	JobStack recentChangesDatabase;
	JobStack recentChangesList;
	// Job in the being undone
	Occupation undoneJob;
	JobPair undoneJobPair;

	// read entries from the file and closes file
	readEntries(rawData, allJobs, headings, columnLengths, jobCounter);
	rawData.close();

	// populating hash table
	for (int i = 0; i < jobCounter; i++) {
		hashTable.insertJob(allJobs[i]);
	}

	// importing list from the text file
	importList(listData, allJobs, list, jobCounter, hashTable);
	listData.close();

	while (toupper(userInput[0]) != 'J') {
		// dynamic array for search list
		Occupation* searchedJobs = new Occupation[jobCounter];
		// dynamic array for sorted list
		Occupation* sortedJobs = nullptr;
		// dynamic array to store pointers of jobs to compare
		Occupation** comparedJobs = nullptr;
		// result pointer to be assigned when searching for a specific index
		Occupation* result = nullptr;
		// node removed when program removes a node from a list
		SinglyLinkedNode<Occupation> nodeRemoved;

		// outputs message to user and takes in input
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
		
		// switch statement based on user input
		switch (menuHandling('A', 'J', userInput)) {
		case 'A': // case A to view entries
			std::cout << "\nDo you want to view the main database or the hash table?" << std::endl
				<< "A: Main Database" << std::endl
				<< "B: Hash Table\n" << std::endl;
			switch (menuHandling('A', 'B', userInput)) {
			case 'A':
				viewEntries(allJobs, headings, columnLengths, jobCounter);
				std::cout << "\nNumber of entries in database: " << jobCounter << std::endl;
				std::cout << "Check output file for full table." << std::endl;
				// calls sorting dialgoue function
				sortingDialogue(allJobs, sortedJobs, jobCounter, headings, columnLengths);
				break;
			case 'B':
				hashTable.printHashTable();
				break;
			}
			break;

		case 'B': // case B to search entries
			std::cout << "\nWhat do you want to searched based on?" << std::endl
				<< "A: Job Title" << std::endl
				<< "B: Wage" << std::endl
				<< "C: Matrix Code" << std::endl
				<< "D: Return to Main Menu\n" << std::endl;

			switch (menuHandling('A', 'D', userInput)) {
			case 'A': // case 1 to search by job title
				searchOutcome = searchFunction(list, "array", "title", searchedJobs, allJobs, jobCounter, headings, columnLengths, searchRows);
				if (searchOutcome) {
					// calls sortingDialogue function if searches were found
					sortingDialogue(searchedJobs, sortedJobs, searchRows, headings, columnLengths);
				}
				break;
			
			case 'B': // case 2 to search by wage
				searchOutcome = searchFunction(list, "array", "wage", searchedJobs, allJobs, jobCounter, headings, columnLengths, searchRows);
				if (searchOutcome) {
					// calls sortingDialogue function if searches were found
					sortingDialogue(searchedJobs, sortedJobs, searchRows, headings, columnLengths);
				}
				break;
			case 'C':
				// buildKeyAndSearch returns nullptr if user wants to return to the main menu
				jobSearchedPtr = buildKeyAndSearch(allJobs, jobCounter, hashTable);
				if (jobSearchedPtr){
					// outputs table headings
					for (int i = 0; i < NUM_OF_HEADINGS; i++) {
						if (i != 1 && i != 15) {
							std::cout << "\t- " << headings[i] << ": ";
							switch (i) {
							case 0:
								std::cout << jobSearchedPtr->getOccupation() << std::endl;
								break;
							case 2:
								std::cout << jobSearchedPtr->getOccupationType() << std::endl;
								break;
							case 3:
								std::cout << jobSearchedPtr->getEmploymentCurrentString() << std::endl;
								break;
							case 4:
								std::cout << jobSearchedPtr->getEmploymentFutureString() << std::endl;
								break;
							case 5:
								std::cout << jobSearchedPtr->getDistributionCurrentString() << std::endl;
								break;
							case 6:
								std::cout << jobSearchedPtr->getDistributionFutureString() << std::endl;
								break;
							case 7:
								std::cout << jobSearchedPtr->getNumericChangeString() << std::endl;
								break;
							case 8:
								std::cout << jobSearchedPtr->getPercentageChangeString() << std::endl;
								break;
							case 9:
								std::cout << jobSearchedPtr->getPercentageChangeString() << std::endl;
								break;
							case 10:
								std::cout << jobSearchedPtr->getJobOpeningsString() << std::endl;
								break;
							case 11:
								std::cout << jobSearchedPtr->getWageString() << std::endl;
								break;
							case 12:
								std::cout << jobSearchedPtr->getEducation() << std::endl;
								break;
							case 13:
								std::cout << jobSearchedPtr->getWorkExperience() << std::endl;
								break;
							case 14:
								std::cout << jobSearchedPtr->getTraining() << std::endl;
								break;
							default:
								break;
							}
						}
					}
				}
				break;
			default: // returns to main menu if user enters "menu"
				break;
			}
			break;

		case 'C': // case C to add an entry
			// asks user what job to add and checks database whether that job exists already
			std::cout << "\nWhat job do you want to add?\n" << std::endl;
			std::getline(std::cin, jobInput);
			lowerString(jobInput);
			searchRows = searchByJob(searchedJobs, allJobs, jobCounter, jobInput);
			// resetting userInput 
			userInput = "";
			
			// outputs existing jobs to console
			if (searchRows > 0) {
				std::cout << std::endl;
				for (int i = 0; i < searchRows; i++) {
					std::cout << "\t- " << searchedJobs[i].getOccupation() << std::endl;
				}
				std::cout << "\nWe have these jobs in our database. Do you still want to add an entry? (y/n)\n" << std::endl;
				userInput = yesOrNoMenu();
			}
			// if statement is only run if user wants to add a job or no existing jobs are found
			if (userInput == "y" || searchRows == 0) {
				// setting secondHalfKey to the smallest value first to track the largest second half key when a user adds a job
				secondHalfKey = "0000";
				// displays the major occupation groups in the table (https://www.bls.gov/oes/2023/may/oes_stru.htm)
				std::cout << std::endl;
				for (int i = 0; i < jobCounter; i++) {
					// major groups are denoted by four 0s at the end of the code, hence a modulo of 10000 will result in 0
					if (allJobs[i].getMatrixCodeInt() % 10000 == 0) {
						std::cout << allJobs[i].getMatrixCode().substr(0, 2) << ": " <<
							allJobs[i].getOccupation() << std::endl;
					}
				}
				while (true) {
					std::cout << "\nEnter a number above to select an existing category for the new occupation.\n" << std::endl;
					std::getline(std::cin, userInput);
					lowerString(userInput);
					try {
						// length check, to make sure that value is contained within an int
						if (userInput.length() < 5) {
							// multiplication by 10000 to search for specific category in the hash table
							searchCode = stoi(userInput) * 10000;
							jobSearchedPtr = hashTable.getJobPointer(searchCode);

							if (jobSearchedPtr != nullptr) {
								// if a job is found, then assign the first 2 digits of the key to firstHalfKey
								firstHalfKey = jobSearchedPtr->getMatrixCode().substr(0, 2);
								// now look for the largest second half of the key
								// comparison operator can be used becase second half of the key is of length 4
								for (int i = 0; i < jobCounter; i++) {
									if (allJobs[i].getMatrixCode().substr(0, 2) == firstHalfKey) {
										if (allJobs[i].getMatrixCode().substr(3, 4) > secondHalfKey) {
											secondHalfKey = allJobs[i].getMatrixCode().substr(3, 4);
										}
									}
								}
								// adds 1 to the last digit of the secondHalfKey
								secondHalfKey = std::to_string(stoi(secondHalfKey) + 1);

								std::cout << "\nThe job's matrix code is: " << firstHalfKey << secondHalfKey << std::endl;
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
				// adds entry to dynamic array
				jobModified = addEntry(allJobs, jobCounter, totalJobsCapacity, jobInput, columnLengths,
					firstHalfKey + "-" + secondHalfKey);
				// pushes job to the recentChanges stack
				recentChangesDatabase.push({ jobModified, "added" });
				// inserts the job to the hash table
				hashTable.insertJob(jobModified);
				// changes save status
				savedDatabase = false;
				// outputs job added to the console
				std::cout << "\n'" << recentChangesDatabase.peek().job.getOccupation()
					<< "' is successfully added to the database." << std::endl;
			}
			break;

		case 'D': // case D to remove an entry
			// asks user what job they want to remove and displays jobs if they are found in the database
			std::cout << "\nHow would you like to search the job to remove?" << std::endl
				<< "A: Job Title" << std::endl
				<< "B: Matrix Code" << std::endl
				<< "C: Return to Main Menu\n" << std::endl;

			switch (menuHandling('A', 'C', userInput)) {
			case 'A':
				// asks user what job they want to remove and displays jobs if they are found in the database
				std::cout << "\nWhat job do you want to remove?\n" << std::endl;
				// if statement is executed if a specific job index was found
				jobSearchedPtr = selectSpecificIndex(searchedJobs, allJobs, jobCounter, userInput, "remove");
				if (jobSearchedPtr) {
					// selectSpecificIndex takes in a reference of userInput which becomes an argument to remove entry
					// this line pushes a job-string pair of the job being removed
					recentChangesDatabase.push({ removeEntry(allJobs, jobCounter, totalJobsCapacity, jobSearchedPtr->getJobIndex()), "removed" });
					// removes job from the hash table
					hashTable.removeJob(*jobSearchedPtr);
					// outputs removed entry to the console
					std::cout << "\n'" << recentChangesDatabase.peek().job.getOccupation()
						<< "' is removed from the database." << std::endl;
				}
				// breaks from case if specific index not found
				break;
			case 'B':
				// jobSearchedPtr returns null if user enters "menu"
				jobSearchedPtr = buildKeyAndSearch(allJobs, jobCounter, hashTable);
				if (jobSearchedPtr) {
					recentChangesDatabase.push({ removeEntry(allJobs, jobCounter, totalJobsCapacity, jobSearchedPtr->getJobIndex()), "removed" });
					// removes job from the hash table and array and pushes it to the stack
					hashTable.removeJob(*jobSearchedPtr);
					// changes save status
					savedDatabase = false;
					// outputs removed entry to the console
					std::cout << "\n'" << recentChangesDatabase.peek().job.getOccupation()
						<< "' is removed from the database." << std::endl;
				}
				break;
			default:
				break;
			}
			break;
		case 'E': // print entire stack to the console
			if (recentChangesDatabase.printEntireStack(columnLengths[0], "Database")) {
				std::cout << "\nDo you want to search for a particular job that was added or removed? (y/n)\n" << std::endl;
				switch (yesOrNoMenu()) {
				case 'y':
					std::cout << "\nEnter the job that you want to search:\n" << std::endl;
					getline(std::cin, jobInput);
					recentChangesDatabase.searchStack(jobInput);
					break;
				case 'n':
					break;
				default:
					break;
				}
			}
			break;

		case 'F': // case F to undo most recent change
			try {
				if (recentChangesDatabase.peek().recentState == "added") {
					undoneJob = recentChangesDatabase.pop().job;
					removeEntry(allJobs, jobCounter, totalJobsCapacity, undoneJob.getJobIndex());
					hashTable.removeJob(undoneJob);
					std::cout << "\n'" << undoneJob.getOccupation() << "' is removed from the database again." << std::endl;
				}
				else if (recentChangesDatabase.peek().recentState == "removed") {
					undoneJob = recentChangesDatabase.pop().job;
					addEntryAgain(undoneJob, allJobs, jobCounter);
					hashTable.insertJob(undoneJob);
					std::cout << "\n'" << undoneJob.getOccupation() << "' is restored to the database." << std::endl;
				}
			}
			catch(const std::out_of_range& e){
				std::cout << "\n" << e.what() << std::endl;
			}
			break;
		case 'G': // case G saves all changes to rawData.txt
			rewriteJobFile(allJobs, jobCounter);
			savedDatabase = true;
			std::cout << "\nSuccessfully saved changes to rawData.txt." << std::endl;
			break;
		
		case 'H':
			while (toupper(userInput[0]) != 'I') {
				// list management user interface
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

				// switch statement to handle user input
				switch (menuHandling('A', 'I', userInput)) {
				case 'A':
					// prints list to output file and console
					list->printList(headings, columnLengths);
					break;
				case 'B':
					// searches for occupations in the list
					std::cout << "\nDo you want to search based on occupation title or wage?" << std::endl
						<< "A: Job Title" << std::endl
						<< "B: Wage" << std::endl
						<< "C: Return to List Menu\n" << std::endl;
					switch (menuHandling('A', 'C', userInput)) {
					case 'A':
						searchOutcome = searchFunction(list, "list", "title", searchedJobs, allJobs, jobCounter, headings, columnLengths, searchRows);
						if (searchOutcome) {
							// calls sortingDialogue function if searches were found
							sortingDialogue(searchedJobs, sortedJobs, searchRows, headings, columnLengths);
						}
						break;
					case 'B':
						searchOutcome = searchFunction(list, "list", "wage", searchedJobs, allJobs, jobCounter, headings, columnLengths, searchRows);
						if (searchOutcome) {
							// calls sortingDialogue function if searches were found
							sortingDialogue(searchedJobs, sortedJobs, searchRows, headings, columnLengths);
						}
						break;
					case 'C':
						break;
					default:
						break;
					}
					break;
				case 'C':
					// CODE MOVED TO MENU HANDLER
				case 'D':
					// calls the remove function and checks if current node count is -1
					nodeRemoved = list->removeFromList();
					if (nodeRemoved.data.getJobIndex() != -1) {
						std::cout << "\n'" << nodeRemoved.data.getOccupation() << "' is successfully removed from the list." << std::endl;
						savedList = false;
						recentChangesList.push({ nodeRemoved.data, "removed", nodeRemoved.data.getJobIndex() });
					}
					break;
				case 'E':
					if (recentChangesList.printEntireStack(columnLengths[0], "List")) {
						std::cout << "\nDo you want to search for a particular job that was added or removed? (y/n)\n" << std::endl;
						switch (yesOrNoMenu()) {
						case 'y':
							std::cout << "\nEnter the job that you want to search:\n" << std::endl;
							getline(std::cin, jobInput);
							recentChangesList.searchStack(jobInput);
							break;
						case 'n':
							break;
						default:
							break;
						}
					}
					break;
				case 'F':
					try {
						if (recentChangesList.peek().recentState == "added") {
							undoneJobPair = recentChangesList.pop();
							list->removeByIndex(undoneJobPair.jobIndex);
							std::cout << "\n'" << undoneJobPair.job.getOccupation() << "' is removed from the list again." << std::endl;
						}
						else if (recentChangesList.peek().recentState == "removed") {
							undoneJobPair = recentChangesList.pop();
							list->append(&undoneJobPair.job);
							std::cout << "\n'" << undoneJobPair.job.getOccupation() << "' is restored to the list." << std::endl;
						}
					}
					catch (const std::out_of_range& e) {
						std::cout << "\n" << e.what() << std::endl;
					}
					break;
				case 'G':
					rewriteListFile(list);
					savedList = true;
					std::cout << "\nSuccessfully saved changes to listData.txt." << std::endl;
					break;
				case 'H':
					// deletes previous linked list and creates a new one
					std::cout << "\nThis will delete any existing lists and create a new one. Do you want to proceed? (y/n)\n" << std::endl;
					switch (yesOrNoMenu()) {
					case 'y': // overwrites original file with new dynamic array
						delete list;
						list = nullptr;
						list = new SinglyLinkedList;
						savedList = false;
						std::cout << "\nList created successfully." << std::endl;
						break;
					case 'n': // returns to main menu
						break;
					default:
						break;
					}
					break;
				case 'I':
					break;
				default:
					break;
				}
			}
			// resets userInput
			userInput = "-";
			break;
		case 'I':
			// Part 1: Taking in how many jobs to compare
			std::cout << "\nHow many occupations do you want to compare?" << std::endl;
			while (true) {
				// try catch block to reject invalid input
				try {
					jobsToCompare = 0;
					// while loop to make sure that the number of jobs fall within viable range
					while ((jobsToCompare < 2 || jobsToCompare > 10) && jobsToCompare <= jobCounter) {
						std::cout << "\nSelect from 2 to 10 occupations.\n" << std::endl;
						std::getline(std::cin, userInput);
						jobsToCompare = stoi(userInput);
					}
					break;
				}
				catch (const std::invalid_argument) {
					std::cout << "\nThe input provided is not an integer. Try again.\n" << std::endl;
				}
			}
			// Part 2: Populating the comparedJobs array
			comparedJobs = new Occupation * [jobsToCompare];
			selectionCounter = 0;
			while (selectionCounter < jobsToCompare){
				jobSearchedPtr = nullptr;
				std::cout << "\nHow would you like to search the job to compare? You have selected " << selectionCounter << "/" << jobsToCompare <<
					" jobs at the moment." << std::endl
					<< "A: Job Title" << std::endl
					<< "B: Matrix Code" << std::endl
					<< "C: Return to Main Menu\n" << std::endl;
				switch (menuHandling('A', 'C', userInput)) {
				case 'A':
					// asks user what job they want to remove and displays jobs if they are found in the database
					std::cout << "\nWhat job do you want to select?\n" << std::endl;
					// if statement is executed if a specific job index was found
					jobSearchedPtr = selectSpecificIndex(searchedJobs, allJobs, jobCounter, userInput, "select");
					// if job is found, then assign the pointer to the array and increment selectionCount
					if (jobSearchedPtr) {
						comparedJobs[selectionCounter] = jobSearchedPtr;
						std::cout << "\n" << jobSearchedPtr->getOccupation() << " is added to the selection." << std::endl;
						selectionCounter++;
					}
					break;
				case 'B':
					// jobSearchedPtr returns null if user enters "menu"
					jobSearchedPtr = buildKeyAndSearch(allJobs, jobCounter, hashTable);
					if (jobSearchedPtr) {
						comparedJobs[selectionCounter] = jobSearchedPtr;
						selectionCounter++;
						std::cout << "\n" << jobSearchedPtr->getOccupation() << " is added to the selection." << std::endl;
					}
					break;
				case 'C':
					std::cout << "\nThis will cancel the job selection process. Are you sure you want to continue? (y/n)\n" << std::endl;
					switch (yesOrNoMenu()) {
					case 'y':
						// assigns a number greater than jobs to compare to exit the loop
						selectionCounter = jobsToCompare + 5;
						break;
					case 'n':
						break;
					}
					break;
				}
			}
			// Part 3: Creating the Statistics Dashboard
			// initial check to see if user exits the program
			// this part uses lambdas, I learned it from Cherno on YouTube https://www.youtube.com/watch?v=mWgmBBz0y8c
			if (selectionCounter == jobsToCompare) {
				// iterate through all headings but skip ones that are not important to the comparison
				for (int i = 1; i < NUM_OF_HEADINGS - 1; i++) {
					std::cout << "\n\n" << headings[i] << ":" << std::endl
						<< "(Numbers in thousands, except percentages and median annual wages)\n" << std::endl;
					switch (i) {
					case 1:
						for (int j = 0; j < jobsToCompare; j++) {
							std::cout << "[" << j << "]: " << comparedJobs[j]->getMatrixCode() << std::endl;
						}
						break;
					case 2:
						for (int j = 0; j < jobsToCompare; j++) {
							std::cout << "[" << j << "]: " << comparedJobs[j]->getOccupationType() << std::endl;
						}
						break;
					case 3:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getEmploymentCurrent(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 4:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getEmploymentFuture(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 5:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getDistributionCurrent(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 6:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getDistributionFuture(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 7:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getNumericChange(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 8:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getPercentageChange(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 9:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getPercentSelfEmployed(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 10:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getJobOpenings(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 11:
					{
						// finds the max value and divides it by the max bar chart length for scaling
						auto getterLambda = [](Occupation* job) {return job->getWage(); };
						printBarChart(comparedJobs,
							findMax(comparedJobs, jobsToCompare, getterLambda),
							MAX_BAR_CHART_LENGTH,
							jobsToCompare,
							getterLambda);
					}
						break;
					case 12:
						for (int j = 0; j < jobsToCompare; j++) {
							std::cout << "[" << j << "]: " << comparedJobs[j]->getEducation() << std::endl;
						}
						break;
					case 13:
						for (int j = 0; j < jobsToCompare; j++) {
							std::cout << "[" << j << "]: " << comparedJobs[j]->getWorkExperience() << std::endl;
						}
						break;
					case 14:
						for (int j = 0; j < jobsToCompare; j++) {
							std::cout << "[" << j << "]: " << comparedJobs[j]->getTraining() << std::endl;
						}
						break;
					}
					std::cout << "\nReference:" << std::endl;
					for (int j = 0; j < jobsToCompare; j++) {
						std::cout << "[" << j << "]: " << comparedJobs[j]->getOccupation() << std::endl;
					}
				}
			}
			break;
		case 'J': // case J breaks out of the case statement, and stops the while loop.
			// if statements to check if there are unsaved changes
			// check list then database, or database then list. Abandon next check if user does not want to exit the program
			if (savedDatabase == false && recentChangesDatabase.getCurrentLength() > 0 ||
				savedList == false && recentChangesList.getCurrentLength() > 0) {
				if (savedDatabase == false && recentChangesDatabase.getCurrentLength() > 0 &&
					savedList == false && recentChangesList.getCurrentLength() > 0) {
					std::cout << "\nThere may be unsaved changes in the main database and your list. Do you still want to exit the program? (y/n)\n" << std::endl;
					switch (yesOrNoMenu()) {
					case 'y':
						break;
					case 'n':
						userInput = "-";
						break;
					}
				}
				else if (savedDatabase == false && recentChangesDatabase.getCurrentLength() > 0) {
					std::cout << "\nThere may be unsaved changes in the main database. Do you still want to exit the program? (y/n)\n" << std::endl;
					switch (yesOrNoMenu()) {
					case 'y':
						break;
					case 'n':
						userInput = "-";
						break;
					}
				}
				else {
					std::cout << "\nThere may be unsaved changes in your list. Do you still want to exit the program? (y/n)\n" << std::endl;
					switch (yesOrNoMenu()) {
					case 'y':
						break;
					case 'n':
						userInput = "-";
						break;
					}
				}
			}
			break;
		default:
			break;
		}
		// delete searchedJobs and sortedJobs dynamic arrays every time the user goes to the main menu
		delete[] searchedJobs;
		searchedJobs = nullptr;
		delete[] sortedJobs;
		sortedJobs = nullptr;
		delete[] comparedJobs;
		comparedJobs = nullptr;
		std::cout << "\nReturning to main menu..." << std::endl;
	}
	std::cout << "\nGoodbye!" << std::endl;
	// deletes allJobs dynamic array
	delete[] allJobs;
	allJobs = nullptr;
	// deletes list
	delete list;
	list = nullptr;
	return 0;
}