#include <iomanip>
#include <chrono>
#include <cctype>
#include "Helpers.h"

float toFloat(const std::string &s) {
    try {
        return stof(s);
    } catch (std::exception &) {
        return 0.0f;
    }
}

void capitalizeFirst(std::string &input) {
    if (!input.empty()) {
        input[0] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(input[0]))
        );
    }
}

void lowerString(std::string &input) {
    if (!input.empty()) {
        for (size_t i = 0; i < input.size(); i++) {
            input[i] = static_cast<char>(
                std::tolower(static_cast<unsigned char>(input[i]))
            );
        }
    }
}

float findMax(DynamicArray<const Occupation *> &array, const std::function<float(const Occupation *)> &fn) {
    float maxValue = 0;
    const int numberOfJobs = array.getCurrentSize();
    for (int i = 0; i < numberOfJobs; i++) {
        if (maxValue < fn(array[i])) {
            maxValue = fn(array[i]);
        }
    }
    return maxValue;
}