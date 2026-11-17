//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Karen Connors
// Date        : 10/09/2024
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : 7-1 Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Define the Course structure to represent a course with its number, title, and prerequisites
struct Course {
    string courseNumber; // The unique identifier for the course
    string title;        // The title of the course
    vector<string> prerequisites; // List of course prerequisites

    // Method to print course details
    void printDetails() const {
        cout << courseNumber << ", " << title << endl;
        // Check if there are any prerequisites and display them
        if (!prerequisites.empty()) {
            cout << "Prerequisites: " << joinPrerequisites() << endl;
        }
        else {
            cout << "Prerequisites: None" << endl; // If no prerequisites, inform the user
        }
    }

private:
    // Helper function to join prerequisites into a single comma-separated string
    string joinPrerequisites() const {
        string result;
        for (const string& prereq : prerequisites) {
            result += prereq + ", "; // Append each prerequisite followed by a comma
        }
        result = result.substr(0, result.length() - 2); // Remove last comma and space
        return result; // Return the formatted string of prerequisites
    }
};

// Forward declare the function prototypes to be defined later
void loadCoursesFromFile(const string& fileName, unordered_map<string, Course>& courseMap);
void printCourseList(const unordered_map<string, Course>& courseMap);
void printCourseDetails(const unordered_map<string, Course>& courseMap);
void displayMenu();

int main() {
    // Create a hash table to store courses, mapped by their course numbers
    unordered_map<string, Course> courseMap;
    int choice;

    cout << "Welcome to the course planner." << endl; // Greet the user
    do {
        displayMenu(); // Show menu options to the user
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice; // Get user's choice

        // Perform actions based on user's choice
        switch (choice) {
        case 1: { // Load courses from a file
            string fileName;
            cout << "Enter the course data file name (Default is CourseInfo.csv): ";
            cin >> fileName; // Prompt for the file name
            loadCoursesFromFile(fileName, courseMap); // Load courses into the hash map
            break; // Exit the case
        }
        case 2: { // Print the list of courses
            printCourseList(courseMap); // Display the list of courses
            break; // Exit the case
        }
        case 3: { // Print details of a specific course
            printCourseDetails(courseMap); // Show details of a selected course
            break; // Exit the case
        }
        case 9: // Exit the program
            cout << "Thank you for using the course planner!" << endl;
            break; // Exit the case
        default: // Handle invalid menu options
            cout << choice << " is not a valid option." << endl;
        }

    } while (choice != 9); // Continue until the user chooses to exit

    return 0; // End of the program
}

// Function to display the main menu options to the user
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl; // Option to load course data
    cout << "2. Print Course List." << endl; // Option to print the course list
    cout << "3. Print Course." << endl; // Option to display details of a specific course
    cout << "9. Exit" << endl; // Option to exit the program
}

// Function to load courses from a specified file into the courseMap
void loadCoursesFromFile(const string& fileName, unordered_map<string, Course>& courseMap) {
    ifstream file(fileName); // Open the specified course data file
    if (!file.is_open()) { // Check if the file was successfully opened
        cout << "Error: The file cannot be opened." << endl;
        return; // Exit if the file can't be opened
    }

    string line;
    // Read the file line by line
    while (getline(file, line)) {
        stringstream ss(line); // Create a string stream for parsing the line
        string courseNumber, title;
        // Extract course number and title from the line
        getline(ss, courseNumber, ',');
        getline(ss, title, ',');
        Course course; // Create a Course object
        course.courseNumber = courseNumber; // Set the course number
        course.title = title; // Set the course title

        // Read prerequisites from the line
        string prereq;
        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(prereq); // Add each prerequisite to the course
        }

        // Store the course in the map using its course number as the key
        courseMap[courseNumber] = course;
    }
    file.close(); // Close the file after reading
}

// Function to print the list of courses stored in courseMap
void printCourseList(const unordered_map<string, Course>& courseMap) {
    vector<Course> courses; // Create a vector to hold courses for sorting
    // Collect all course values from the map
    for (const auto& pair : courseMap) {
        courses.push_back(pair.second);
    }

    // Sort courses by courseNumber using a lambda expression
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
        });

    cout << "Here is a sample schedule:" << endl << endl; // Display a header
    // Print each course in the sorted list
    for (const Course& course : courses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }
}

// Function to print the details of a specific course
void printCourseDetails(const unordered_map<string, Course>& courseMap) {
    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber; // Get the course number from the user

    // Convert course number to uppercase to maintain consistency
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    auto it = courseMap.find(courseNumber); // Search for the course in the map
    if (it != courseMap.end()) { // If course is found
        it->second.printDetails(); // Print the details of the course
    }
    else {
        cout << "Course not found." << endl; // Inform the user if the course is not found
    }
}