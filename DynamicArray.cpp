#include <string>
#include <fstream>
#include "DynamicArray.h"

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

template <typename T>
Occupation &DynamicArray<T>::operator[](int index) const {
    return *data[index];
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
    auto newData = new std::unique_ptr<Occupation>[newCapacity];

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
Occupation *&DynamicArray<T>::addJobToArray(const Occupation &jobToAdd) {
    data[currentSize] = jobToAdd;
    currentSize++;
    return data[currentSize - 1];
}

template <typename T>
void DynamicArray<T>::addEntryAgain(const Occupation &jobAdded) {
    int indexAdded = jobAdded.getJobIndex();
    // iterates from the last job in the array (currentSize always < capacity in this case)
    for (int i = currentSize - 1; i >= indexAdded; i--) {
        data[i].setJobIndex(data[i].getJobIndex() + 1);
        data[i + 1] = data[i];
    }
    data[indexAdded] = jobAdded;
    currentSize++;
}

template <typename T>
Occupation DynamicArray<T>::removeEntry(const int indexRemoved) {
    // remove entry and shifts elements to the left
    for (int i = indexRemoved + 1; i < currentSize; i++) {
        data[i].setJobIndex(data[i].getJobIndex() - 1);
        data[i - 1] = data[i];
    }

    currentSize--;
    return data[indexRemoved];
}

template <typename T>
void DynamicArray<T>::rewriteInputFile(std::fstream &modifiedData) {
    for (int i = 0; i < currentSize; i++) {
        modifiedData << data[i].getOccupation() << std::endl
                << data[i].getMatrixCode() << std::endl
                << data[i].getOccupationType() << std::endl
                << data[i].getEmploymentCurrentString() << std::endl
                << data[i].getEmploymentFutureString() << std::endl
                << data[i].getDistributionCurrentString() << std::endl
                << data[i].getDistributionFutureString() << std::endl
                << data[i].getNumericChangeString() << std::endl
                << data[i].getPercentageChangeString() << std::endl
                << data[i].getPercentSelfEmployedString() << std::endl
                << data[i].getJobOpeningsString() << std::endl
                << data[i].getWageString() << std::endl
                << data[i].getEducation() << std::endl
                << data[i].getWorkExperience() << std::endl
                << data[i].getTraining() << std::endl
                << data[i].getHandbookContent() << std::endl;
    }
    // closes file
    modifiedData.close();
}
