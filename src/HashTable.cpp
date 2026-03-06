#include "HashTable.h"


OpenAddressingBucket::OpenAddressingBucket() {
	JobPointer = nullptr;
}
OpenAddressingBucket::OpenAddressingBucket(const Occupation *bucketJobPointer) {
	JobPointer = bucketJobPointer;
}

OpenAddressingBucket OpenAddressingBucket::EMPTY_SINCE_START;
OpenAddressingBucket OpenAddressingBucket::EMPTY_AFTER_REMOVAL;

bool OpenAddressingBucket::isEmpty() const {
	return this == &EMPTY_SINCE_START || this == &EMPTY_AFTER_REMOVAL;
}
bool OpenAddressingBucket::isEmptyAfterRemoval() const {
	return this == &EMPTY_AFTER_REMOVAL;
}
bool OpenAddressingBucket::isEmptySinceStart() const {
	return this == &EMPTY_SINCE_START;
}

HashTable::HashTable() :
	table(nullptr),
	tableCapacity(0){}

HashTable::HashTable(size_t initialCapacity) {
	table = new OpenAddressingBucket*[initialCapacity];
	for (int i = 0; i < initialCapacity; i++) {
		table[i] = &OpenAddressingBucket::EMPTY_SINCE_START;
	}
	this->tableCapacity = initialCapacity;
}

HashTable::~HashTable() {
	for (int i = 0; i < tableCapacity; i++) {
		if (!table[i]->isEmpty()) {
			delete table[i];
		}
	}
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
size_t HashTable::hashJobKey(int key) {
	int R = 24;
	int squaredKey = key * key;
	
	int lowBitsToRemove = (32 - R) / 2;
	size_t extractedBits = squaredKey >> lowBitsToRemove;
	extractedBits = extractedBits & (0xFFFFFFFF >> (32 - R));

	return extractedBits;
}

size_t HashTable::getTableCapacity() const {
	return tableCapacity;
}

const Occupation* HashTable::getJobPointer(int key) const {
	size_t hashedKey = hashJobKey(key);
	for (int i = 0; i < tableCapacity; i++) {
		size_t bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
		if (table[bucketIndex]->isEmptySinceStart()) {
			return nullptr;
		}
		if (!table[bucketIndex]->isEmptyAfterRemoval()) {
			if (table[bucketIndex]->JobPointer->getMatrixCodeInt() == key) {
				return table[bucketIndex]->JobPointer;
			}
		}
	}
	// returns nullptr if table is full
	return nullptr;
}

// insert method calls private hashing method
// Since the hashtable only points to the main array, this implementation rejects identical keys.
bool HashTable::insertJob(Occupation *jobInserted) const{

	size_t hashedKey = hashJobKey(jobInserted->getMatrixCodeInt());

	// track the first deleted index for better performance as further searches
	// for the same key will be shorter
	size_t firstDeletedIndex = -1;
	for (int i = 0; i < tableCapacity; i++) {
		size_t bucketIndex = (c2 * i * i + c1 * i + hashedKey) % tableCapacity;
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
		if (table[bucketIndex]->isEmptySinceStart()) {
			return false;
		}
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