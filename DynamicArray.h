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

    // overloading [] operator to access job at that index
    Occupation& operator[](int index) const;

    DynamicArray& operator=(const DynamicArray&) = delete;
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    int getCurrentSize() const;
    int getCapacity() const;

    void increaseCapacity();

    Occupation *addJobToArray(T jobToAdd, int index);

    // function to remove an entry
    void removeEntry(const Occupation *jobToRemove);
};