cmake_minimum_required(VERSION 4.1)
project(bls_labor_data_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bls_labor_data_parser
        input/listData.txt
        input/rawData.txt
        output/output.txt
        HashTable.cpp
        HashTable.h
        Helpers.cpp
        Helpers.h
        OldDatabase.cpp
        OldDatabase.h
        LinkedList.cpp
        LinkedList.h
        main.cpp
        Occupation.cpp
        Occupation.h
        Sort.h
        Stack.cpp
        Stack.h
        menuHandler.h
        menuHandler.cpp
        DynamicArray.h
        DynamicArray.cpp
        JobDatabase.cpp
        JobDatabase.h
)
