// LinkedList.h
// Darren Daniel

#pragma once
#include "Occupation.h"
#include "DynamicArray.h"
#include "Stack.h"

class SinglyLinkedNode {
public:
    // each node will store a pointer to the main occupation objects (unique ptrs held by dynamic array)
    const Occupation *data;
    SinglyLinkedNode *next = nullptr;
};

// linked list class
class SinglyLinkedList {
private:
    SinglyLinkedNode *head = nullptr;
    SinglyLinkedNode *tail = nullptr;
    int listSize = 0;

public:
    ~SinglyLinkedList();
    void clear();

    void append(const Occupation *jobAppended);

    void prepend(const Occupation *jobPrepended);

    void insertAfter(const Occupation *jobInserted, int jobIndex);

    SinglyLinkedNode *removeByIndex(int jobIndex);

    bool searchListByJob(const std::string &jobToSearch, DynamicArray<const Occupation *> &searchedJobsArray);

    bool searchListByWage(int lowerLimit, int upperLimit,
                          DynamicArray<const Occupation *> &searchedJobsArray);

    bool searchListByIndex(int jobIndex);

    // list iterator for functions in MenuHandler
    void forEachJobInList(const std::function<void(const Occupation *job)> &fn) const;

    int getListSize() const;

};
