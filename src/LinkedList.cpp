#include <iostream>
#include <fstream>
#include <chrono>
#include "Helpers.h"
#include "LinkedList.h"

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

void SinglyLinkedList::append(const Occupation *jobAppended) {
    auto newNode = new SinglyLinkedNode;
    newNode->data = jobAppended;
    // assigns head and tail to the new node if the list is empty
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    listSize++;
}

void SinglyLinkedList::prepend(const Occupation *jobPrepended) {
    auto newNode = new SinglyLinkedNode;
    newNode->data = jobPrepended;
    // assigns head and tail to the new node if the list is empty
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        newNode->next = head;
        head = newNode;
    }
    listSize++;
}

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

    std::cout << "Could not find node to remove." << std::endl;
    return nullptr;
}

bool SinglyLinkedList::searchListByJob(const std::string &jobToSearch, DynamicArray<const Occupation *> &searchedJobsArray) const {

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

bool SinglyLinkedList::searchListByWage(const float lowerLimit, const float upperLimit,
                                       DynamicArray<const Occupation *> &searchedJobsArray) const {

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

bool SinglyLinkedList::searchListByIndex(int jobIndex) const {
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

int SinglyLinkedList::getListSize() const {
    return listSize;
}

