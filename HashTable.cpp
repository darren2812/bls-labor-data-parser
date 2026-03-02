#include <iostream>
#include <iomanip>
#include <unordered_set>
#include "HashTable.h"


// defining constructors for open addressing buckets
OpenAddressingBucket::OpenAddressingBucket() {
	JobPointer = nullptr;
}
// job pointer does not need
OpenAddressingBucket::OpenAddressingBucket(Occupation *bucketJobPointer) {
	JobPointer = bucketJobPointer;
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

HashTable::HashTable() :
	table(nullptr),
	tableCapacity(0){}

HashTable::HashTable(int initialCapacity) {
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
	// no need to iterate through hash table because pointers are managed by the dynamic array and job database classes
	delete[] table;
	table = nullptr;
}
OpenAddressingBucket *HashTable::operator[](int index) const {
	return table[index];
}

HashTable& HashTable::operator=(HashTable&& other) noexcept {
	delete[] table;

	if (this != &other) {
		this->c1 = other.c1;
		this->c2 = other.c2;
		this->tableCapacity = other.tableCapacity;
		this->table = other.table;

		other.c1 = 0;
		other.c2 = 0;
		other.tableCapacity = 0;
		other.table = nullptr;
	}

	return *this;
}

// mid-square hash function copied from zyBooks
int HashTable::hashJobKey(int key) const {
	int R = 24;
	// although this might cause overflow ChatGPT says it is handled by the compiler
	int squaredKey = key * key;
	
	int lowBitsToRemove = (32 - R) / 2;
	int extractedBits = squaredKey >> lowBitsToRemove;
	extractedBits = extractedBits & (0xFFFFFFFF >> (32 - R));

	return extractedBits;
}

int HashTable::getTableCapacity() const {
	return tableCapacity;
}

// function to get job pointer
const Occupation* HashTable::getJobPointer(int key) const {
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
bool HashTable::insertJob(Occupation *jobInserted) {
	int hashedKey = hashJobKey(jobInserted->getMatrixCodeInt());
	int bucketIndex;
	// suggestion from ChatGPT to track the first deleted index for better performance as further searches 
	// for the same key will be shorter
	int firstDeletedIndex = -1;
	for (int i = 0; i < tableCapacity; i++) {
		bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
		if (table[bucketIndex]->isEmptySinceStart()) {
			if (firstDeletedIndex == -1) {
				table[bucketIndex] = new OpenAddressingBucket(jobInserted);
			}
			else {
				table[firstDeletedIndex] = new OpenAddressingBucket(jobInserted);
			}
			return true;
		}
		if (table[bucketIndex]->isEmptyAfterRemoval() && i == 0) {
			firstDeletedIndex = bucketIndex;
		}
		else if (!table[bucketIndex]->isEmptyAfterRemoval()) {
			if (table[bucketIndex]->JobPointer->getMatrixCodeInt() == jobInserted->getMatrixCodeInt()) {
				return false;
			}
		}
	}
	// loop finishes once table is full
	return false;
}

// remove method takes in a key as its argument
bool HashTable::removeJob(int matrixCodeInt) {
	int hashedKey = hashJobKey(matrixCodeInt);
	int bucketIndex;
	for (int i = 0; i < tableCapacity; i++) {
		bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
		// item is not in table if bucket is pointing to the empty since start sentinel value
		if (table[bucketIndex]->isEmptySinceStart()) {
			return false;
		}
		// changes the bucket index to empty after removal. deletion happens in the main array
		if (!table[bucketIndex]->isEmptyAfterRemoval()) {
			if (table[bucketIndex]->JobPointer->getMatrixCodeInt() == matrixCodeInt) {
				delete table[bucketIndex];
				table[bucketIndex] = &OpenAddressingBucket::EMPTY_AFTER_REMOVAL;
				return true;
			}
		}
	}
	// loop finishes once table is full
	return false;
}