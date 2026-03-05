# Labor Market Data Parser (C++)

## Overview
A C++ console application that parses and organizes labor market data from the U.S. Bureau of Labor Statistics. Users can search, sort, add, remove, compare, and save occupation data.

## Features
- Search occupations by title, matrix code, or wage
- Sort data by job title, wage, typical education required for entry, and on-the-job training.
- Compare multiple occupations side-by-side.
- Save occupation data to a persistent text file.
- Undo changes made.
- Make a personalized list of occupations.

## Data Structures Used
- Templated Dynamic Array (std::unique_ptr and Occupation *)
- Hash Table (Open Addressing, Quadratic Probing)
- Singly Linked List
- Stack

## How It Works
- Occupation records are stored in a dynamically allocated array of std::unique_ptr<Occupation> objects. All other data structures, except for the stack for removals, store raw Occupation pointers.
- A hash table is used to search an occupation by its SOC matrix code.
- User-selected jobs are stored in a linked list.
- Addition and removal of jobs in both the linked list and dynamic array can be tracked and undone using a stack.
- Removals use std::move to move the occupation from main array to the stack.
- Search and sort arrays allow for the same printing functions to be reused across all data structure search / sort results.