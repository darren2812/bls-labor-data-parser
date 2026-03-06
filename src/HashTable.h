#pragma once
#include "Occupation.h"

class OpenAddressingBucket {
public:
	// open addressing bucket only stores a job pointer.
	// The key is already a member field of an Occupation object
	// this means that occupation objects do not need to be copied as all data is contained in the main array
	const Occupation* JobPointer;

	// declaring constructors and destructor
	OpenAddressingBucket();
	OpenAddressingBucket(const Occupation *bucketJobPointer);
	~OpenAddressingBucket() = default;

	// Two sentinel buckets based on zyBooks implementation
	static OpenAddressingBucket EMPTY_SINCE_START;
	static OpenAddressingBucket EMPTY_AFTER_REMOVAL;

	// functions to determine bucket state from zyBooks
	bool isEmpty() const;
	bool isEmptyAfterRemoval() const;
	bool isEmptySinceStart() const;
};

class HashTable {
	OpenAddressingBucket** table;
	// declaring constants for quadratic probing
	int c1 = 1;
	int c2 = 1;
	size_t tableCapacity;

	// private hashing method
	static size_t hashJobKey(int key);
public:
	HashTable();
	HashTable(size_t initialCapacity);
	~HashTable();

	OpenAddressingBucket* operator[](size_t index) const;

	HashTable& operator=(const HashTable&) = delete;
	HashTable& operator=(HashTable&&) noexcept;

	size_t getTableCapacity() const;
	
	const Occupation* getJobPointer(int key) const;

	bool insertJob(const Occupation *jobInserted) const;
	bool removeJob(int matrixCodeInt);
};