#pragma once

#include "Occupation.h"

template <typename T>
class DynamicArray {
private:
    T data = nullptr;
    int capacity = 0;
    int currentSize = 0;

public:
    DynamicArray() = default;
    ~DynamicArray();
    DynamicArray(int capacity);
    DynamicArray(const DynamicArray&) = delete;
    DynamicArray(DynamicArray&& other) noexcept;

    // overloading [] operator to access job at that index
    T& operator[](int index);
    const T& operator[](int index) const;

    DynamicArray& operator=(const DynamicArray&) = delete;
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    int getCurrentSize() const;
    int getCapacity() const;

    void increaseCapacity();

    Occupation *addJobToMainArray(std::unique_ptr<Occupation> jobToAdd, int index);
    void append(const Occupation* jobToAppend);

    // function to remove an entry
    T removeEntry(int indexRemoved);
};