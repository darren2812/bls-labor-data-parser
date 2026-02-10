#include <iostream>
#include <iomanip>
#include <unordered_set>
#include "HashTable.h"


// defining constructors for open addressing buckets
OpenAddressingBucket::OpenAddressingBucket() {
	JobPointer = nullptr;
}
// chatGPT helped debug dynamic memory allocation for JobPointer
OpenAddressingBucket::OpenAddressingBucket(Occupation& bucketJobPointer) {
	JobPointer = new Occupation(bucketJobPointer);
}
OpenAddressingBucket::~OpenAddressingBucket() {
	delete JobPointer;
	JobPointer = nullptr;
}

// definition of static variables (ChatGPT helped debugged)
OpenAddressingBucket OpenAddressingBucket::EMPTY_SINCE_START;
OpenAddressingBucket OpenAddressingBucket::EMPTY_AFTER_REMOVAL;

// returns a boolean based on whether the current object has the same memory address as the sentinel bucket
bool OpenAddressingBucket::isEmpty() const {
	return this == &EMPTY_SINCE_START || this == &EMPTY_AFTER_REMOVAL;
}
bool OpenAddressingBucket::isEmptyAfterRemoval() const {
	return this == &EMPTY_AFTER_REMOVAL;
}
bool OpenAddressingBucket::isEmptySinceStart() const {
	return this == &EMPTY_SINCE_START;
}

// defining constructor for hash table
HashTable::HashTable(int c1, int c2, int initialCapacity) {
	table = new OpenAddressingBucket*[initialCapacity];
	for (int i = 0; i < initialCapacity; i++) {
		table[i] = &OpenAddressingBucket::EMPTY_SINCE_START;
	}
	this->c1 = c1;
	this->c2 = c2;
	this->tableCapacity = initialCapacity;
}

// defining destructor for the hash table (code based on zyBooks)
HashTable::~HashTable() {
	for (int i = 0; i < tableCapacity; i++) {
		if (!table[i]->isEmpty()) {
			delete table[i];
		}
	}
	delete[] table;
	table = nullptr;
}

// mid-square hash function copied from zyBooks
int HashTable::hashJobKey(int key) {
	int R = 24;
	// although this might cause overflow ChatGPT says it is handled by the compiler
	int squaredKey = key * key;
	
	int lowBitsToRemove = (32 - R) / 2;
	int extractedBits = squaredKey >> lowBitsToRemove;
	extractedBits = extractedBits & (0xFFFFFFFF >> (32 - R));

	return extractedBits;
}

// function to get job pointer
Occupation* HashTable::getJobPointer(int key) {
	int hashedKey = hashJobKey(key);
	int bucketIndex;
	for (int i = 0; i < tableCapacity; i++) {
		bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
		if (table[bucketIndex]->isEmptySinceStart()) {
			return nullptr;
		}
		else if (!table[bucketIndex]->isEmptyAfterRemoval()) {
			if (table[bucketIndex]->JobPointer->getMatrixCodeInt() == key) {
				return table[bucketIndex]->JobPointer;
			}
		}
	}
	// loop finishes once table is full
	return nullptr;
}

// insert method calls private hashing method
// Since the hashtable only points to the main array, this implementation rejects identical keys.
bool HashTable::insertJob(Occupation& jobInserted) {
	int hashedKey = hashJobKey(jobInserted.getMatrixCodeInt());
	int bucketIndex;
	// suggestion from ChatGPT to track the first deleted index for better performance as further searches 
	// for the same key will be shorter
	int firstDeletedIndex = -1;
	for (int i = 0; i < tableCapacity; i++) {
		bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
		if (table[bucketIndex]->isEmptySinceStart()) {
			if (firstDeletedIndex == -1) {
				// chatGPT helped clear a key misunderstanding of how insertion creates a new OpenAddressingBucket instead of 
				// modifying the JobPointer directly
				table[bucketIndex] = new OpenAddressingBucket(jobInserted);
			}
			else {
				table[firstDeletedIndex] = new OpenAddressingBucket(jobInserted);
			}
			return true;
		}
		else if (table[bucketIndex]->isEmptyAfterRemoval() && i == 0) {
			firstDeletedIndex = bucketIndex;
		}
		else if (!table[bucketIndex]->isEmptyAfterRemoval()) {
			if (table[bucketIndex]->JobPointer->getMatrixCodeInt() == jobInserted.getMatrixCodeInt()) {
				return false;
			}
		}
	}
	// loop finishes once table is fuill
	return false;
}

// remove method takes in a key as its argument
bool HashTable::removeJob(Occupation& jobRemoved) {
	int hashedKey = hashJobKey(jobRemoved.getMatrixCodeInt());
	int bucketIndex;
	for (int i = 0; i < tableCapacity; i++) {
		bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
		// item is not in table if bucket is pointing to the empty since start sentinel value
		if (table[bucketIndex]->isEmptySinceStart()) {
			return false;
		}
		// changes the bucket index to empty after removal. deletion happens in the main array
		else if (!table[bucketIndex]->isEmptyAfterRemoval()) {
			if (table[bucketIndex]->JobPointer->getMatrixCodeInt() == jobRemoved.getMatrixCodeInt()) {
				delete table[bucketIndex];
				table[bucketIndex] = &OpenAddressingBucket::EMPTY_AFTER_REMOVAL;
				return true;
			}
		}
	}
	// loop finishes once table is full
	return false;
}

void HashTable::printHashTable() {
	short indexWidth = 7;
	short keyWidth = 10;

	std::cout << std::left << std::setw(indexWidth) << "Index" << "|"
		<< std::setw(keyWidth) << "Code" << "|"
		<< "Occupation" << std::endl;
	for (int i = 0; i < tableCapacity; i++) {
		if (!table[i]->isEmpty()) {
			std::cout << std::left << std::setw(indexWidth) << i << "|"
				<< std::setw(keyWidth) << table[i]->JobPointer->getMatrixCode() << "|"
				<< table[i]->JobPointer->getOccupation() << std::endl;
		}
		else {
			std::cout << std::left << std::setw(indexWidth) << i << "|"
				<< std::setw(keyWidth) << "-" << "|"
				<< "-" << std::endl;
		}
	}
}