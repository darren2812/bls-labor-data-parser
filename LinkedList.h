// LinkedList.h
// Darren Daniel

#pragma once
#include "Occupation.h"
#include "DynamicArray.h"
#include "Stack.h"

class SinglyLinkedNode {
public:
    // each node will store a pointer to the main occupation objects (uniqueptrs held by dynamic array)
    Occupation *data;
    SinglyLinkedNode *next = nullptr;
};

// linked list class
class SinglyLinkedList {
private:
    SinglyLinkedNode *head = nullptr;
    SinglyLinkedNode *tail = nullptr;
    int listSize = 0;
    int nodeCounter = 0;

public:
    ~SinglyLinkedList();

    void append(Occupation *jobAppended);

    void prepend(Occupation *jobPrepended);

    void insertAfter(Occupation *jobInserted, int jobIndex);

    SinglyLinkedNode *removeByIndex(int jobIndex);

    bool searchListByJob(const std::string &jobToSearch, DynamicArray<Occupation *> &searchedJobsArray);

    bool searchListByWage(const float &lowerLimit, const float &upperLimit,
                          DynamicArray<Occupation *> &searchedJobsArray);

    bool searchListByIndex(int jobIndex);

    // list iterator for functions in MenuHandler
    void forEachJobInList(const std::function<void(Occupation *job)> &fn) const;

    // getters
    int getListSize() const;

};
