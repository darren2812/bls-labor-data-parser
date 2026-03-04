// LinkedList.cpp
// Darren Daniel

#include <iostream>
#include <fstream>
#include <chrono>
#include "Helpers.h"
#include "LinkedList.h"

// destructor for linked list
SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

void SinglyLinkedList::clear() {
    SinglyLinkedNode *current = head;
    while (current) {
        SinglyLinkedNode *nodeToDelete = current;
        current = current->next;
        delete nodeToDelete;
    }
    head = nullptr;
    tail = nullptr;
    listSize = 0;
}

// function to append node
void SinglyLinkedList::append(const Occupation *jobAppended) {
    // creates a new node and assigns its data
    auto newNode = new SinglyLinkedNode;
    // dereferences jobAppended pointer and assigns to data field
    newNode->data = jobAppended;
    // assigns head and tail to new node if the list is empty
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    // sets the next of tail to new node and reassigns tail to new node
    else {
        tail->next = newNode;
        tail = newNode;
    }
    listSize++;
}

// function to prepend node
void SinglyLinkedList::prepend(const Occupation *jobPrepended) {
    // creates a new node and assigns its data
    auto newNode = new SinglyLinkedNode;
    newNode->data = jobPrepended;
    // assigns head and tail to new node if the list is empty
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    // changes the next pointer of new node to point to head and reassigns head
    else {
        newNode->next = head;
        head = newNode;
    }
    listSize++;
}

// function to insert a node after another node
void SinglyLinkedList::insertAfter(const Occupation *jobInserted, int jobIndex) {
    SinglyLinkedNode *current = head;
    while (current) {
        if (current->data && current->data->getJobIndex() == jobIndex) {
            auto newNode = new SinglyLinkedNode;
            newNode->next = current->next;
            current->next = newNode;
            newNode->data = jobInserted;
            listSize++;
            return;
        }
        current = current->next;
    }
}

// removes node based on node counter value
const Occupation *SinglyLinkedList::removeByIndex(int jobIndex) {
    SinglyLinkedNode *current = head;
    SinglyLinkedNode *before = nullptr;
    while (current && current->data->getJobIndex() != jobIndex) {
        before = current;
        current = current->next;
    }
    if (current) {
        if (current == head) {
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
        } else if (current == tail) {
            tail = before;
            tail->next = nullptr;
        } else if (before != nullptr) {
            before->next = current->next;
        }

        const Occupation *occupationToReturn = current->data;
        delete current;
        listSize--;

        return occupationToReturn;
    }

    // output message if node is not found
    std::cout << "Could not find node to remove." << std::endl;
    return nullptr;
}

bool SinglyLinkedList::searchListByJob(const std::string &jobToSearch, DynamicArray<const Occupation *> &searchedJobsArray) {

    searchedJobsArray = DynamicArray<const Occupation*>(listSize);

    SinglyLinkedNode *current = head;

    std::string query = jobToSearch;
    lowerString(query);

    while (current != nullptr) {

        if (current->data != nullptr) {
            std::string currentEntry = current->data->getOccupation();
            lowerString(currentEntry);
            if (query.find(currentEntry) != std::string::npos) {
                searchedJobsArray.append(current->data);
            }
        }

        current = current->next;
    }

    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}

// function to search by wage
bool SinglyLinkedList::searchListByWage(const int lowerLimit, const int upperLimit,
                                       DynamicArray<const Occupation *> &searchedJobsArray) {

    searchedJobsArray = DynamicArray<const Occupation*>(listSize);

    SinglyLinkedNode* current = head;
    while (current != nullptr) {
        if (current->data && current->data->getWage() >= lowerLimit && current->data->getWage() <= upperLimit) {
            searchedJobsArray.append(current->data);
        }
        current = current->next;
    }

    if (searchedJobsArray.getCurrentSize() > 0) {
        return true;
    }
    return false;
}

bool SinglyLinkedList::searchListByIndex(int jobIndex) {
    SinglyLinkedNode * current = head;

    while (current) {
        if (current->data && current->data->getJobIndex() == jobIndex) {
            return true;
        }
        current = current->next;
    }

    return false;
}

void SinglyLinkedList::forEachJobInList(const std::function<void(const Occupation *job)> &fn) const {
    SinglyLinkedNode *current = head;
    while (current) {
        if (current->data != nullptr) {
            fn(current->data);
        }
        current = current->next;
    }
}

// function to get size of list
int SinglyLinkedList::getListSize() const {
    return listSize;
}

