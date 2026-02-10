#pragma once
#include <string>
#include "Occupation.h"

class OpenAddressingBucket {
public:
	// open addressing bucket only stores a job pointer because the key is already a member field of an Occupation object
	// this means that occupation objects do not need to be copied as all data is contained in the main array
	Occupation* JobPointer;

	// declaring constructors and destructor
	OpenAddressingBucket();
	OpenAddressingBucket(Occupation& bucketJobPointer);
	~OpenAddressingBucket();

	// Two sentinal buckets based on zyBooks implementation
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
	int c1;
	int c2;
	int tableCapacity;

	// private hashing method
	int hashJobKey(int key);
public:
	// default constructor takes in default arguments if not defined by the user
	HashTable(int c1 = 1, int c2 = 1, int initialCapacity = 2003);
	// chatGPT informed that a destructor is required as the array is dynamically allocated
	~HashTable();
	
	// search method
	Occupation* getJobPointer(int key);

	// methods to modify contents of table
	bool insertJob(Occupation& jobInserted);
	bool removeJob(Occupation& jobRemoved);

	// method to display all contents of the hash table
	void printHashTable();
};