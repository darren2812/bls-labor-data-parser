// LinkedList.h
// Darren Daniel

#pragma once
#include "Occupation.h"
#include "Stack.h"

// function template for node to take in any data type
template<typename T>
class SinglyLinkedNode {
public:
	// each node will store an Occupation object 
	T data;
	SinglyLinkedNode* next = nullptr;
};

// linked list class
class SinglyLinkedList {
private:
	SinglyLinkedNode<Occupation>* head = nullptr;
	SinglyLinkedNode<Occupation>* tail = nullptr;
	int listSize = 0;
	int nodeCounter = 0;
public:
	~SinglyLinkedList();
	void append(Occupation* jobAppended);
	void prepend(Occupation* jobPrepended);
	bool insertAfter(Occupation* jobInserted);
	SinglyLinkedNode<Occupation> removeFromList();
	SinglyLinkedNode<Occupation> removeByIndex(int jobIndex);
	int searchListByJob(std::string& jobSearched, Occupation* searchedJobs);
	int searchListByWage(const float& lowerLimit, const float& upperLimit, Occupation* searchedJobs);
	bool searchListByCounter(int nodeCount);
	void printList(const std::string* headings, int* columnLengths);
	
	// getters
	int getListSize() const;
	int getNodeCounter() const;
	SinglyLinkedNode<Occupation>* getListHead();

	// incrementers and decrementers
	void incrementListSize();
	void decrementListSize();
	void incrementNodeCounter();
};