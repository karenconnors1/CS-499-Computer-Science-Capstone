# Course Planner Application
## CS 499 Milestone Three — Algorithms and Data Structures Enhancement Two

Author: Karen Connors
Course: CS 499 — Computer Science Capstone
Instructor: Professor Joseph Conlan
Original Course: CS 300 — Data Structures and Algorithms
Original Artifact Date: October 9, 2024
Enhancement Date: November 14, 2026
Programming Language: C++17

## Project Overview

The Course Planner Application is a C++ program originally developed for CS 300. It allows users to load course information from a CSV file, display an alphanumeric course list, and retrieve course details and prerequisites using an unordered_map.

For the CS 499 Computer Science Capstone, the application was enhanced to analyze complex prerequisite relationships using a directed graph and depth-first search (DFS).

The enhanced application retains the original hash-table-based course storage while introducing algorithms that identify multilevel prerequisites, detect missing course references, identify circular dependencies, and generate a valid prerequisite-first course completion sequence.

Additional improvements include filtering empty prerequisite fields from CSV input and validating numeric menu selections to prevent infinite repetition after invalid input.

This enhancement demonstrates Course Outcome 3, algorithmic problem-solving, and Course Outcome 4, computing practices and software engineering.


## Implemented Enhancements

### 1. Directed Prerequisite Graph

The enhanced application models course dependencies using a directed graph implemented with an unordered_map of course identifiers and prerequisite lists.

Each course represents a vertex, and each prerequisite relationship represents a directed edge from a course to its prerequisite.

The buildPrerequisiteGraph() function constructs this graph from the existing course records while preserving the original unordered_map used for course information and lookups.

### 2. Depth-First Search (DFS)

The depthFirstSearch() function recursively explores the direct and indirect prerequisites of a selected course.

The algorithm uses two unordered_set collections:

- visited: Tracks courses whose dependencies have been fully processed.
- active: Tracks courses currently being explored in the recursive search.

These collections prevent unnecessary repeated processing and support circular dependency detection.

### 3. Missing Prerequisite Detection

The DFS algorithm verifies that each referenced prerequisite exists in the graph.

When a required course is missing, the application reports the missing course identifier and does not generate an invalid completion sequence.

### 4. Circular Dependency Detection

A circular dependency occurs when a course eventually depends on itself through one or more prerequisite relationships.

When DFS encounters a course already in the active set, it identifies the dependency cycle, displays an error message, and stops the sequence generation.

### 5. Prerequisite-First Course Sequencing

The application adds each course to the completion sequence only after processing all its prerequisites.

This postorder DFS approach produces a valid prerequisite-respecting sequence for the selected course when no missing references or cycles are encountered.

Users access this functionality through menu option 4, Analyze Course Prerequisites.

### 6. Input Validation and Reliability

The CSV-loading function was improved to remove surrounding whitespace from prerequisite identifiers and ignore empty prerequisite fields.

The main menu was also enhanced to detect nonnumeric input, clear the input stream's error state, and allow users to enter another selection without causing an endless loop.

## Algorithmic Complexity Analysis

The enhanced Course Planner Application uses an unordered_map to store course information and an adjacency-list representation of a directed graph to model prerequisite relationships.

For the following analysis:

- V represents the number of courses (vertices).
- E represents the number of prerequisite relationships (edges).

### Graph Construction

The buildPrerequisiteGraph() function iterates through the stored courses and copies their prerequisite lists into the graph.

- Average-case time complexity: O(V + E)
- Space complexity: O(V + E)

Each course is processed, and each prerequisite relationship is stored as an edge.

### Depth-First Search

The depthFirstSearch() function recursively explores prerequisite relationships.

The visited set prevents previously completed courses from being processed repeatedly, while the active set identifies circular dependencies.

- Average-case time complexity: O(V + E)
- Auxiliary space complexity: O(V)

Each reachable course and prerequisite relationship is processed at most once during a successful traversal. The visited and active sets, recursive call stack, and completion sequence require space proportional to the number of reachable vertices.

### Overall Prerequisite Analysis

The application first constructs the graph and then performs DFS on the selected course's prerequisite dependencies.

- Average-case time complexity: O(V + E)
- Total space complexity: O(V + E)

These average-case bounds assume expected constant-time lookups and insertions in the unordered_map and unordered_set containers.

The enhancement improves the application's ability to analyze multilevel prerequisite dependencies efficiently while detecting invalid prerequisite relationships.

## Testing and Validation

The enhanced Course Planner Application was compiled and tested on macOS using Clang with the C++17 standard.

The following tests were completed during development.

| Test | Test Input | Actual Result | Status |
|---|---|---|---|
| Compilation | C++17 with warning flags | Compiled without errors or warnings | Passed |
| Multilevel prerequisites | CSCI400 using CourseInfo.csv | Generated an eight-course prerequisite-first sequence | Passed |
| No prerequisites | CSCI100 using CourseInfo.csv | Generated a single-course sequence | Passed |
| Missing prerequisite | CSCI300 using Test_MissingPrerequisite.csv | Identified missing course CSCI999 | Passed |
| Circular dependency | CSCI300 using Test_CircularPrerequisite.csv | Identified circular dependency involving CSCI300 | Passed |
| Invalid course selection | CSCI999 using CourseInfo.csv | Displayed "Course not found: CSCI999" | Passed |
| Data not loaded | Option 4 before loading a file | Requested that course data be loaded first | Passed |
| Missing CSV file | Enhanced/DoesNotExist.csv | Reported that the file could not be opened | Passed |
| Invalid menu input | abc | Displayed an error, returned to the menu, and accepted option 9 | Passed |

### Test Data

The original CourseInfo.csv file was preserved in the Original directory.

Two additional CSV files were created in the Enhanced directory:

- Test_MissingPrerequisite.csv: Contains an intentionally nonexistent prerequisite, CSCI999.
- Test_CircularPrerequisite.csv: Contains a three-course dependency cycle.

These files allow the error-detection behavior to be reproduced without modifying the original artifact.

### Validation Summary

Testing confirmed that the application can analyze multilevel prerequisites, generate a prerequisite-respecting course sequence, and identify missing or circular dependencies.

Additional testing verified error handling for invalid course selections, missing data files, unloaded course data, and nonnumeric menu input.

The invalid menu input test initially exposed an endless repetition defect. This issue was corrected by validating the input stream, clearing failed input, and discarding invalid characters before requesting another selection.

## Build and Run Instructions

### Requirements

- A C++17-compatible compiler
- A terminal or command-line environment
- The enhanced C++ source file and accompanying CSV test files

The application was developed and tested on macOS using the Clang compiler.

### Compile the Application

From the main CS499_Milestone3_EnhancementTwo project directory, run:

```bash
clang++ -std=c++17 -Wall -Wextra -pedantic Enhanced/ProjectTwo_Enhanced.cpp -o Enhanced/course_planner
```

This command compiles the enhanced source file and creates the course_planner executable inside the Enhanced directory.

### Run the Application

From the same project directory, run:

```bash
./Enhanced/course_planner
```

### Application Menu

The enhanced application provides the following options:
- 1. Load Data Structure
- 2. Print Course List
- 3. Print Course
- 4. Analyze Course Prerequisites
- 9. Exit

### Load the Original Course Data

Select option 1 and enter:

```text
Original/CourseInfo.csv
```

Then select option 4 and enter a course number, such as:

```text
CSCI400
```

The application analyzes the requested course and displays a prerequisite-first completion sequence when the dependency relationships are valid.

### Reproduce the Missing Prerequisite Test

Select option 1 and load:

```text
Enhanced/Test_MissingPrerequisite.csv
```

Then select option 4 and analyze CSCI300.

Expected result:

```text
Unable to generate a valid course sequence.
Error: Missing course or prerequisite: CSCI999
```

### Reproduce the Circular Dependency Test

Select option 1 and load:

```text
Enhanced/Test_CircularPrerequisite.csv
```

Then select option 4 and analyze CSCI300.

Expected result:

```text
Unable to generate a valid course sequence.
Error: Circular prerequisite dependency involving: CSCI300
```

### Invalid Menu Input

Entering nonnumeric input, such as abc, produces an error message and returns control to the menu without an endless loop.

Enter 9 to exit the application.

## Project File Structure

The project maintains separate Original and Enhanced directories to preserve the original CS 300 artifact and document the completed CS 499 enhancement.

```text
CS499_Milestone3_EnhancementTwo/
|
|-- Original/
|   |-- CourseInfo.csv
|   |-- ProjectTwo.cpp
|   |-- ProjectTwo.sln
|   |-- ProjectTwo.vcxproj
|   |-- ProjectTwo.vcxproj.filters
|   |-- ProjectTwo.vcxproj.user
|   |-- README.md
|
|-- Enhanced/
    |-- ProjectTwo_Enhanced.cpp
    |-- README.md
    |-- Test_MissingPrerequisite.csv
    |-- Test_CircularPrerequisite.csv
    |-- course_planner
```

### Original Directory

Contains the original CS 300 project files, which are retained for comparison with the enhanced artifact.

### Enhanced Directory

Contains the enhanced C++17 source code, technical documentation, and test files demonstrating prerequisite analysis and error detection.

The course_planner file is a compiled macOS executable generated from ProjectTwo_Enhanced.cpp. It can be rebuilt using the compilation instructions above.

## CS 499 Course Outcome Alignment

### Course Outcome 3 — Algorithmic Problem-Solving

The enhancement demonstrates algorithmic problem-solving through directed graph modeling, recursive depth-first search, missing prerequisite validation, circular dependency detection, and prerequisite-first course sequencing.

The use of adjacency lists and hash-based containers supports efficient analysis, with average-case O(V + E) time complexity for graph construction and traversal.

### Course Outcome 4 — Computing Practices and Software Engineering

The enhancement demonstrates computing practices through preserving the original artifact, organizing source and test files, documenting technical decisions, validating user input, troubleshooting defects, and conducting repeatable functional testing.

## Enhancement Summary

This enhancement extends the original Course Planner Application beyond direct prerequisite lookup to support recursive dependency analysis and valid course sequencing.

The original CS 300 project remains preserved, while the enhanced application provides additional functionality, reliability improvements, and documented algorithmic complexity suitable for inclusion in the CS 499 professional ePortfolio.
