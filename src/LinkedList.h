#pragma once
#include "Occupation.h"
#include "DynamicArray.h"
#include "Stack.h"

class SinglyLinkedNode {
public:
    const Occupation *data = nullptr;
    SinglyLinkedNode *next = nullptr;
};

class SinglyLinkedList {
    SinglyLinkedNode *head = nullptr;
    SinglyLinkedNode *tail = nullptr;
    int listSize = 0;

public:
    ~SinglyLinkedList();
    void clear();

    void append(const Occupation *jobAppended);

    void prepend(const Occupation *jobPrepended);

    void insertAfter(const Occupation *jobInserted, int jobIndex);

    const Occupation *removeByIndex(int jobIndex);

    bool searchListByJob(const std::string &jobToSearch, DynamicArray<const Occupation *> &searchedJobsArray) const;

    bool searchListByWage(float lowerLimit, float upperLimit,
                          DynamicArray<const Occupation *> &searchedJobsArray) const;

    bool searchListByIndex(int jobIndex) const;

    // list iterator for functions in MenuHandler
    void forEachJobInList(const std::function<void(const Occupation *job)> &fn) const;

    int getListSize() const;

};
