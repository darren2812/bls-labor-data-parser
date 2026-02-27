// LinkedList.h
// Darren Daniel

#pragma once
#include "Occupation.h"
#include "Stack.h"

class SinglyLinkedNode {
public:
	// each node will store a pointer to the main occupation objects (uniqueptrs held by dynamic array)
	Occupation* data;
	SinglyLinkedNode* next = nullptr;
};

// linked list class
class SinglyLinkedList {
private:
	SinglyLinkedNode* head = nullptr;
	SinglyLinkedNode* tail = nullptr;
	int listSize = 0;
	int nodeCounter = 0;
public:
	~SinglyLinkedList();
	void append(Occupation* jobAppended);
	void prepend(Occupation* jobPrepended);
	void insertAfter(Occupation* jobInserted, int jobIndex);
	SinglyLinkedNode removeFromList();
	SinglyLinkedNode removeByIndex(int jobIndex);
	int searchListByJob(std::string& jobSearched, Occupation* searchedJobs);
	int searchListByWage(const float& lowerLimit, const float& upperLimit, Occupation* searchedJobs);
	bool searchListByIndex(int nodeCount);

	void forEachJobInList(const std::function<void(Occupation *&job)> &fn) const;

	// getters
	int getListSize() const;
	int getNodeCounter() const;
	SinglyLinkedNode* getListHead();

	// incrementers and decrementers
	void incrementListSize();
	void decrementListSize();
	void incrementNodeCounter();
};