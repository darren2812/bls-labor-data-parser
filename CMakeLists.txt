cmake_minimum_required(VERSION 4.1)
project(bls_labor_data_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bls_labor_data_parser
        input/listData.txt
        input/rawData.txt
        output/output.txt
        src/HashTable.cpp
        src/HashTable.h
        src/Helpers.cpp
        src/Helpers.h
        src/LinkedList.cpp
        src/LinkedList.h
        src/main.cpp
        src/Occupation.cpp
        src/Occupation.h
        src/Sort.h
        src/Stack.cpp
        src/Stack.h
        src/MenuHandler.h
        src/MenuHandler.cpp
        src/DynamicArray.h
        src/JobDatabase.cpp
        src/JobDatabase.h
)
