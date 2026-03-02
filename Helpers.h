// Helpers.h
// Darren Daniel

#pragma once

#include <string>
#include <functional>
#include "DynamicArray.h"

// input handler for missing values
float toFloat(std::string &s);

// input handler to standardize input case
void capitalizeFirst(std::string &input);

void lowerString(std::string &input);

// function to find max value in an array
float findMax(DynamicArray<const Occupation *> &array,
              const std::function<float(const Occupation *)> &fn);
