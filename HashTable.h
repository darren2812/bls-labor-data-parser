#pragma once
#include "Occupation.h"

class OpenAddressingBucket {
public:
	// open addressing bucket only stores a job pointer because the key is already a member field of an Occupation object
	// this means that occupation objects do not need to be copied as all data is contained in the main array
	Occupation* JobPointer;

	// declaring constructors and destructor
	OpenAddressingBucket();
	OpenAddressingBucket(Occupation *bucketJobPointer);
	~OpenAddressingBucket();

	// Two sentinel buckets based on zyBooks implementation
	static OpenAddressingBucket EMPTY_SINCE_START;
	static OpenAddressingBucket EMPTY_AFTER_REMOVAL;

	// functions to determine bucket state from zyBooks
	bool isEmpty() const;
	bool isEmptyAfterRemoval() const;
	bool isEmptySinceStart() const;
};

class HashTable {
private:
	// main structure is a dynamic array of pointers
	OpenAddressingBucket** table;
	// declaring constants for quadratic probing
	int c1 = 1;
	int c2 = 1;
	int tableCapacity;

	// private hashing method
	int hashJobKey(int key) const;
public:
	// default constructor takes in default arguments if not defined by the user
	HashTable();
	HashTable(int initialCapacity);
	// chatGPT informed that a destructor is required as the array is dynamically allocated
	~HashTable();

	// overloading the [] operator for printing
	OpenAddressingBucket* operator[](int index) const;

	HashTable& operator=(const HashTable&) = delete;
	HashTable& operator=(HashTable&&) noexcept;

	int getTableCapacity() const;
	
	// search method
	Occupation* getJobPointer(int key) const;

	// methods to modify contents of table
	bool insertJob(Occupation *jobInserted);
	bool removeJob(Occupation& jobRemoved);
};