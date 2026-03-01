#include <string>
#include <fstream>
#include "DynamicArray.h"

#include <iostream>

template<typename T>
DynamicArray<T>::DynamicArray(int capacity) : capacity(capacity) {
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray &&other) noexcept{
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

template<typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& other) noexcept
 : data(other.data), capacity(other.capacity), currentSize(other.currentSize) {
    other.data = nullptr;
    other.capacity = 0;
    other.currentSize = 0;
}

template <typename T>
T &DynamicArray<T>::operator[](int index) {
    return data[index];
}

template <typename T>
const T &DynamicArray<T>::operator[](int index) const {
    return data[index];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
    data = nullptr;
}

template <typename T>
void DynamicArray<T>::increaseCapacity() {
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

template <typename T>
int DynamicArray<T>::getCurrentSize() const {
    return currentSize;
}

template <typename T>
int DynamicArray<T>::getCapacity() const {
    return capacity;
}

template <typename T>
Occupation *DynamicArray<T>::addJobToMainArray(std::unique_ptr<Occupation> jobToAdd, const int index) {
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

template<typename T>
void DynamicArray<T>::append(const Occupation *jobToAppend) {
    if (currentSize >= capacity) {
        increaseCapacity();
    }

    data[currentSize] = jobToAppend;

    currentSize++;
    return data[currentSize - 1];
}

template <typename T>
T DynamicArray<T>::removeEntry(int indexRemoved) {

    T jobRemoved = data[indexRemoved];

    if (indexRemoved < 0 || indexRemoved >= currentSize) {
        std::cout << "Error: index out of bounds" << std::endl;
        return nullptr;
    }

    for (int i = indexRemoved; i < currentSize - 1; i++) {
        data[i] = data[i + 1];
        data[i]->setJobIndex(i);
    }
    currentSize--;
    return jobRemoved;
}


