// Helpers.cpp
// Darren Daniel

#include <iomanip>
#include <chrono>
#include <cctype>
#include "Helpers.h"

// ChatGPT suggested using a try and catch statement to handle missing values (i.e. "-")
float toFloat(std::string &s) {
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
        for (int i = 0; i < input.length(); i++) {
            input[0] = static_cast<char>(
                std::tolower(static_cast<unsigned char>(input[0]))
            );
        }
    }
}

// function to mind max value in an array
float findMax(DynamicArray<const Occupation *> &array, int numberOfJobs,
              const std::function<float(const Occupation *)> &function) {
    float maxValue = 0;
    for (int i = 0; i < numberOfJobs; i++) {
        if (maxValue < function(array[i])) {
            maxValue = function(array[i]);
        }
    }
    return maxValue;
}
