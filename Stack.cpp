#include "Stack.h"
#include <iostream>
#include <iomanip>

#include "Helpers.h"

// constructor creates a new dynamic array of job pairs
JobStack::JobStack() {
    array = new JobPair[allocationSize];
}

// destructor frees up memory
JobStack::~JobStack() {
    delete[] array;
    array = nullptr;
}

// dynamically resizes the jobstack
void JobStack::resize() {
    const int newSize = allocationSize == 0 ? 1 : allocationSize * 2;
    auto newArray = new JobPair[newSize];

    for (int i = 0; i < currentLength; i++) {
        newArray[i] = std::move(array[i]);
    }

    delete[] array;
    array = newArray;
    allocationSize = newSize;
}

// pushes a pair to the stack and resizes when appropriate
void JobStack::push(JobPair &&pair) {
    if (currentLength >= allocationSize) {
        resize();
    }
    array[currentLength] = std::move(pair);
    currentLength++;
}

JobPair JobStack::pop() {
    if (currentLength <= 0) {
        throw std::out_of_range("No previous changes are recorded.");
    }
    currentLength--;
    return std::move(array[currentLength]);
}

const JobPair &JobStack::peek() const {
    if (currentLength <= 0) {
        throw std::out_of_range("No previous changes are recorded.");
    }
    return array[currentLength - 1];
}

// getter to check length
int JobStack::getCurrentLength() const {
    return currentLength;
}

void JobStack::forEachJobInStack(const std::function<void(const JobPair &pair)> &fn) const {
    for (int i = 0; i < currentLength; i++) {
        fn(array[i]);
    }
}
