#pragma once

#include <string>
#include <functional>
#include "DynamicArray.h"

float toFloat(std::string &s);

void capitalizeFirst(std::string &input);

void lowerString(std::string &input);

float findMax(DynamicArray<const Occupation *> &array,
              const std::function<float(const Occupation *)> &fn);
