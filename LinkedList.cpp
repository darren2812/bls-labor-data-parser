// LinkedList.cpp
// Darren Daniel

#include <iostream>
#include <fstream>
#include <chrono>
#include "Helpers.h"
#include "LinkedList.h"

// destructor for linked list
SinglyLinkedList::~SinglyLinkedList() {
	// sets the current node as the head
	SinglyLinkedNode<Occupation>* current = head;
	while (current) {
		// traverses through the list and sets nodeToDelete for each node
		SinglyLinkedNode<Occupation>* nodeToDelete = current;
		current = current->next;
		delete nodeToDelete;
	}
}

// function to append node
void SinglyLinkedList::append(Occupation* jobAppended) {
	// creates a new node and assigns its data
	SinglyLinkedNode<Occupation>* newNode = new SinglyLinkedNode<Occupation>;
	// dereferences jobAppended pointer and assigns to data field
	newNode->data = *jobAppended;
	// assings head and tail to new node if the list is empty
	if (head == nullptr) {
		head = newNode;
		tail = newNode;
	}
	// sets the next of tail to new node and reassings tail to new node
	else {
		tail->next = newNode;
		tail = newNode;
	}
	incrementListSize();
}

// function to prepend node
void SinglyLinkedList::prepend(Occupation* jobPrepended) {
	// creates a new node and assigns its data
	SinglyLinkedNode<Occupation>* newNode = new SinglyLinkedNode<Occupation>;
	newNode->data = *jobPrepended;
	// assigns head and tail to new node if the list is empty
	if (head == nullptr) {
		head = newNode;
		tail = newNode;
	}
	// changes the next pointer of new node to point to head and reassigns head
	else {
		newNode->next = head;
		head = newNode;
	}
	incrementListSize();
}

// function to insert a node after another node
bool SinglyLinkedList::insertAfter(Occupation* jobInserted) {
	if (head == nullptr) {
		std::cout << "\nThere are no jobs to display. Use another method to add the occupation." << std::endl;
		return false;
	}
	// creates a new node and assigns its data
	SinglyLinkedNode<Occupation>* newNode = new SinglyLinkedNode<Occupation>;
	newNode->data = *jobInserted;
	// assigns head to current
	SinglyLinkedNode<Occupation>* current = head;
	int index = 0;
	int targetIndex;
	std::string userInput = "-";
	// traverses through list and prints out each job
	std::cout << std::endl;
	while (current) {
		std::cout << "Index " << index << ": " << current->data.getOccupation() << std::endl;
		current = current->next;
		index++;
	}
	// looping to ensure that user correctly inputs number
	while (true) {
		std::cout << "\nAfter which index do you want to insert the occupation?" << std::endl
			<< "If you want to return to the list menu, enter 'menu'\n" << std::endl;
		std::getline(std::cin, userInput);
		lowerString(userInput);
		if (userInput == "menu") {
			return false;
		}
		if (toFloat(userInput) >= 0 && toFloat(userInput) <= index && g_catch == false)
		{
			break;
		}
	}
	// resets current and index and sets target index
	current = head;
	index = 0;
	targetIndex = stoi(userInput);
	// nodeDeleted traverses through the list
	while (current) {
		// if the indices match, insert current node to the list
		if (index == targetIndex) {
			newNode->next = current->next;
			current->next = newNode;
			incrementListSize();
			return true;
		}
		current = current->next;
		index++;
	}
	// returns false if node cannot be found
	std::cout << "Could not find node to insert after." << std::endl;
	return false;
}

// undo remove  removes node based on node counter value
SinglyLinkedNode<Occupation> SinglyLinkedList::removeByIndex(int jobIndex) {
	SinglyLinkedNode<Occupation>* current = head;
	SinglyLinkedNode<Occupation>* before = nullptr;
	SinglyLinkedNode<Occupation> nodeRemoved;
	while (current && current->data.getJobIndex() != jobIndex) {
		before = current;
		current = current->next;
	}
	if (current) {
		if (current == head) {
			head = head->next;
			// chatGPT suggested changing tail to nullptr if the node removed is the only node
			if (head == nullptr) {
				tail = nullptr;
			}
		}
		else if (current == tail) {
			tail = before;
			// chatGPT helped debugging with this line
			tail->next = nullptr; 
		}
		else {
			before->next = current->next;
		}
		nodeRemoved = *current;
		delete current;
		current = nullptr;
		decrementListSize();
		return nodeRemoved;
	}
	else {
		// output message if node is not found
		std::cout << "Could not find node to remove." << std::endl;
		return SinglyLinkedNode<Occupation>();
	}
}
	
// function to remove a node and returns pointer of the node being removed
SinglyLinkedNode<Occupation> SinglyLinkedList::removeFromList() {
	if (head == nullptr) {
		std::cout << "\nThere are no jobs to remove." << std::endl;
		// returns default constructor if removal unsuccessful
		return SinglyLinkedNode<Occupation>();
	}
	// assigns head to current
	SinglyLinkedNode<Occupation>* current = head;
	// before node that trails nodeDeleted
	SinglyLinkedNode<Occupation>* before = nullptr;
	int targetNodeCount;
	std::string userInput = "-";
	// traverses through list and prints out each job
	std::cout << std::endl;
	while (current) {
		std::cout << "Index " << current->data.getJobIndex() << ": " << current->data.getOccupation() << std::endl;
		current = current->next;
	}
	// looping to ensure that user correctly inputs number
	while (true) {
		std::cout << "\nEnter the index that you want to remove:" << std::endl
			<< "If you want to return to the list menu, enter 'menu'\n" << std::endl;
		std::getline(std::cin, userInput);
		lowerString(userInput);
		if (userInput == "menu") {
			// returns default constructor if search is unsuccessful
			return SinglyLinkedNode<Occupation>();
		}
		try {
			targetNodeCount = stoi(userInput);
			if (searchListByCounter(targetNodeCount)) {
				return removeByIndex(targetNodeCount);
				break;
			}
		}
		catch (std::invalid_argument& e) {
			std::cout << "\n" << e.what() << std::endl;
		}
		catch (std::out_of_range& e) {
			std::cout << "\n" << e.what() << std::endl;
		}
	}
}

int SinglyLinkedList::searchListByJob(std::string& jobSearched, Occupation* searchedJobs) {
	SinglyLinkedNode<Occupation>* current = head;
	int jobCounter = 0;
	while (current != nullptr) {
		// similar code to searching a dynamic array
		// search for jobs that do not start with the word being searched
		jobSearched[0] = tolower(jobSearched[0]);
		// Chat GPT suggested using .find() instead of making own substring search algorithm
		if (current->data.getOccupation().find(jobSearched) != std::string::npos) {
			searchedJobs[jobCounter] = current->data;
			jobCounter++;
		}
		// search for jobs that start with the word being searched (capitalized first letter)
		jobSearched[0] = toupper(jobSearched[0]);
		if (current->data.getOccupation().find(jobSearched) != std::string::npos) {
			searchedJobs[jobCounter] = current->data;
			jobCounter++;
		}
		current = current->next;
	}
	return jobCounter;
 }

// function to search by wage
int SinglyLinkedList::searchListByWage(const float& lowerLimit, const float& upperLimit, Occupation* searchedJobs) {
	 // sets the current pointer of the list to the head
	 SinglyLinkedNode<Occupation>* current = head;
	 int jobCounter = 0;
	 // traverses through the list while current is not null
	 while (current) {
		 // checks if wage is between lower and upper limits inclusive
		 if (current->data.getWage() >= lowerLimit && current->data.getWage() <= upperLimit) {
			 searchedJobs[jobCounter] = current->data;
			 jobCounter++;
		 }
		 current = current->next;
	 }
	 return jobCounter;
 }

bool SinglyLinkedList::searchListByCounter(int nodeCount) {
	// sets the current pointer of the list to the head
	SinglyLinkedNode<Occupation>* current = head;
	while (current) {
		if (current->data.getJobIndex() == nodeCount) {
			return true;
		}
		current = current->next;
	}
	std::cout << "\nThe index you entered was not found." << std::endl;
	return false;
}

// function to print the entire list to the console and output file
void SinglyLinkedList::printList(const std::string* headings, int* columnLengths) {
	// prints message if list is empty
	if (head == nullptr) {
		std::cout << "\nYour list is currently empty." << std::endl;
		return;
	}
	
	// time counters, chrono implementation taken from cpp reference website and ChatGPT
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point endTime;
	std::chrono::microseconds timeTaken;
	// opens output file
	std::ofstream output("output.txt");
	// sets the current node to the head
	SinglyLinkedNode<Occupation>* current = head;
	// prints table headings
	printTableHeadings(output, headings, columnLengths);
	
	// start of search algorithm and outputs time taken in microseconds
	startTime = std::chrono::high_resolution_clock::now();
	// iterates through the list and prints every single entry line by line
	while (current != nullptr) {
		printTableEntry(output, columnLengths, current->data);
		current = current->next;
	}
	endTime = std::chrono::high_resolution_clock::now();
	timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
	
	// close output file
	output.close();
	// reports the number of jobs in the list
	std::cout << "\nThere are currently " << getListSize() << " occupations in your list." << std::endl;
	// reports time taken to traverse through list
	std::cout << "\nTime to traverse and print list: " << timeTaken.count() << " microseconds." << std::endl;
}

// function to get size of list
int SinglyLinkedList::getListSize() const{
	return listSize;
}

// function to get list node counter
int SinglyLinkedList::getNodeCounter() const {
	return nodeCounter;
}

// function to get list head node
SinglyLinkedNode<Occupation>* SinglyLinkedList::getListHead() {
	return head;
}

// function to increment size of list
void SinglyLinkedList::incrementListSize() {
	listSize++;
}

// function to decrement size of list
void SinglyLinkedList::decrementListSize() {
	listSize--;
}

// function to increment list node counter
void SinglyLinkedList::incrementNodeCounter() {
	nodeCounter++;
}