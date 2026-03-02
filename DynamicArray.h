#pragma once

#include "Occupation.h"
#include <iostream>

template<typename T>
class DynamicArray {
    T *data = nullptr;
    int capacity = 0;
    int currentSize = 0;

public:
    DynamicArray() = default;

    ~DynamicArray() {
        delete[] data;
        data = nullptr;
    }

    DynamicArray(int capacity) : capacity(capacity) {
        data = new T[capacity];
    }

    DynamicArray(const DynamicArray &) = delete;

    DynamicArray(DynamicArray &&other) noexcept
        : data(other.data), capacity(other.capacity), currentSize(other.currentSize) {
        other.data = nullptr;
        other.capacity = 0;
        other.currentSize = 0;
    }

    // overloading [] operator to access job at that index
    T &operator[](int index) {
        return data[index];
    }

    const T &operator[](int index) const {
        return data[index];
    }

    DynamicArray &operator=(const DynamicArray &) = delete;

    DynamicArray &operator=(DynamicArray &&other) noexcept {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            currentSize = other.currentSize;
            data = other.data;
            other.capacity = 0;
            other.currentSize = 0;
            other.data = nullptr;
        }
        return *this;
    }

    int getCurrentSize() const {
        return currentSize;
    }

    int getCapacity() const {
        return capacity;
    }

    void increaseCapacity() {
        if (currentSize < capacity) return;

        const int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        auto newData = new T[newCapacity];

        for (int i = 0; i < currentSize; i++) {
            newData[i] = std::move(data[i]);
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    Occupation *addJobToMainArray(std::unique_ptr<Occupation> jobToAdd, int index) {
        if (currentSize >= capacity) {
            increaseCapacity();
        }

        // shift right
        for (int i = currentSize; i > index; i--) {
            data[i] = std::move(data[i - 1]);
            data[i]->setJobIndex(i);
        }

        data[index] = std::move(jobToAdd);
        data[index]->setJobIndex(index);

        currentSize++;
        return data[index].get();
    }

    void append(const Occupation *jobToAppend) {
        if (currentSize >= capacity) {
            increaseCapacity();
        }

        data[currentSize] = jobToAppend;

        currentSize++;
    }

    // function to remove an entry
    T removeEntry(int indexRemoved) {
        T jobRemoved = std::move(data[indexRemoved]);

        if (indexRemoved < 0 || indexRemoved >= currentSize) {
            std::cout << "Error: index out of bounds" << std::endl;
            return nullptr;
        }

        for (int i = indexRemoved; i < currentSize - 1; i++) {
            data[i] = std::move(data[i + 1]);
            data[i]->setJobIndex(i);
        }
        currentSize--;
        return std::move(jobRemoved);
    }
};
