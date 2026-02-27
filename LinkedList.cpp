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
	SinglyLinkedNode* current = head;
	while (current) {
		// traverses through the list and sets nodeToDelete for each node
		SinglyLinkedNode* nodeToDelete = current;
		current = current->next;
		delete nodeToDelete;
	}
}

// function to append node
void SinglyLinkedList::append(Occupation* jobAppended) {
	// creates a new node and assigns its data
	auto newNode = new SinglyLinkedNode;
	// dereferences jobAppended pointer and assigns to data field
	newNode->data = jobAppended;
	// assigns head and tail to new node if the list is empty
	if (head == nullptr) {
		head = newNode;
		tail = newNode;
	}
	// sets the next of tail to new node and reassigns tail to new node
	else {
		tail->next = newNode;
		tail = newNode;
	}
	listSize++;
}

// function to prepend node
void SinglyLinkedList::prepend(Occupation* jobPrepended) {
	// creates a new node and assigns its data
	auto newNode = new SinglyLinkedNode;
	newNode->data = jobPrepended;
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
	listSize++;
}

// function to insert a node after another node
void SinglyLinkedList::insertAfter(Occupation* jobInserted, int jobIndex) {
	SinglyLinkedNode* current = head;
	while (current) {
		if (current->data->getJobIndex() == jobIndex) {
			auto newNode = new SinglyLinkedNode;
			newNode->next = current->next;
			current->next = newNode;
			newNode->data = jobInserted;
			listSize++;
			return;
		}
		current = current->next;
	}
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
			if (searchListByIndex(targetNodeCount)) {
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

bool SinglyLinkedList::searchListByIndex(int nodeCount) {
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

void SinglyLinkedList::forEachJobInList(const std::function<void(Occupation *&job)> &fn) const {
	SinglyLinkedNode* current = head;
	while (current) {
		fn(current->data);
		current = current->next;
	}
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