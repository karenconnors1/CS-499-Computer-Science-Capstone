/*
 * Project: Course Planner Application
 * File: ProjectTwo_Enhanced.cpp
 * Author: Karen Connors
 * Course: CS 499 - Computer Science Capstone
 * Original Artifact: CS 300 - Data Structures and Algorithms
 * Original Project: Project Two (October 9, 2024)
 * Enhancement Category: Algorithms and Data Structures
 * Enhancement Date: November 14, 2026
 *
 * Purpose:
 * Enhance the original Course Planner Application to improve
 * prerequisite dependency analysis, course sequencing,
 * algorithmic efficiency, and reliability for academic advising.
 *
 * Planned Enhancements:
 * - Retain the existing unordered_map for efficient course lookups.
 * - Model courses and prerequisite relationships as a directed graph.
 * - Implement depth-first search (DFS) for multilevel dependencies.
 * - Detect missing prerequisite references.
 * - Identify circular prerequisite dependencies.
 * - Generate a valid prerequisite-respecting course sequence.
 * - Evaluate algorithmic time and space complexity using Big O.
 * - Test courses with no prerequisites, multilevel dependencies,
 *   missing prerequisites, and circular dependencies.
 *
 * Course Outcomes:
 * Outcome 3 - Algorithmic Problem-Solving
 * Outcome 4 - Computing Practices and Software Engineering
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <limits>

using namespace std;

// Stores a course's identifier, title, and direct prerequisite identifiers.
struct Course {
    string courseNumber; // Unique identifier used to look up the course.
    string title;        // Human-readable course title.
    vector<string> prerequisites; // Identifiers of the course's prerequisites.

    // Prints the course and its direct prerequisites.
    void printDetails() const {
        cout << courseNumber << ", " << title << endl;

        // Display the prerequisite list, or explicitly state that it is empty.
        if (!prerequisites.empty()) {
            cout << "Prerequisites: " << joinPrerequisites() << endl;
        }
        else {
            cout << "Prerequisites: None" << endl;
        }
    }

private:
    // Formats the prerequisite identifiers as a comma-separated string.
    string joinPrerequisites() const {
        string result;

        for (const string& prereq : prerequisites) {
            result += prereq + ", ";
        }

        // Remove the separator appended after the last prerequisite.
        result = result.substr(0, result.length() - 2);
        return result;
    }
};


/*
 * Enhancement Two: Directed Prerequisite Graph
 *
 * Courses are vertices. An edge from a course points to one of
 * its prerequisites.
 *
 * The courseMap remains the source of full course details; this
 * graph provides a structure for dependency traversal.
 */
using PrerequisiteGraph = unordered_map<string, vector<string>>;

/*
 * Copies each course and its prerequisite edges into a graph.
 * Courses with no prerequisites are included with an empty edge list.
 *
 * Average-case time: O(V + E)
 * Space: O(V + E)
 * V = number of courses; E = number of prerequisite relationships.
 */
PrerequisiteGraph buildPrerequisiteGraph(
    const unordered_map<string, Course>& courseMap)
{
    PrerequisiteGraph graph;

    // Create one graph entry for every course, including prerequisite-free ones.
    for (const auto& entry : courseMap) {
        const Course& course = entry.second;
        graph[course.courseNumber] = course.prerequisites;
    }

    return graph;
}


/*
 * Enhancement Two: Depth-First Search (DFS)
 *
 * Recursively visits a course's prerequisite dependencies.
 * `visited` records fully processed courses; `active` records
 * courses on the current recursion path.
 *
 * Finding an active course again indicates a cycle. A reference
 * with no corresponding graph entry indicates a missing course.
 * Courses are appended after their prerequisites, producing a
 * prerequisite-first completion order.
 *
 * Average-case time: O(V + E) for the traversed subgraph.
 * Auxiliary space: O(V), excluding the graph itself.
 */
bool depthFirstSearch(
    const string& courseNumber,
    const PrerequisiteGraph& graph,
    unordered_set<string>& visited,
    unordered_set<string>& active,
    vector<string>& completionOrder,
    string& errorMessage)
{
    // A missing graph entry means the course or prerequisite was not loaded.
    auto it = graph.find(courseNumber);

    if (it == graph.end()) {
        errorMessage = "Missing course or prerequisite: " + courseNumber;
        return false;
    }

    // A course already on the current path forms a circular dependency.
    if (active.find(courseNumber) != active.end()) {
        errorMessage = "Circular prerequisite dependency involving: "
            + courseNumber;
        return false;
    }

    // This course and all its prerequisites have already been processed.
    if (visited.find(courseNumber) != visited.end()) {
        return true;
    }

    // Mark the course as being explored on this recursion path.
    active.insert(courseNumber);

    // Visit each direct prerequisite; recursive calls follow multilevel chains.
    for (const string& prerequisite : it->second) {
        if (!depthFirstSearch(
                prerequisite,
                graph,
                visited,
                active,
                completionOrder,
                errorMessage)) {
            return false;
        }
    }

    // Finish this vertex only after all its prerequisites are valid and complete.
    active.erase(courseNumber);
    visited.insert(courseNumber);

    // Postorder appending places prerequisites before the requested course.
    completionOrder.push_back(courseNumber);

    return true;
}

// Function prototypes for functions defined below main().
void loadCoursesFromFile(const string& fileName, unordered_map<string, Course>& courseMap);
void printCourseList(const unordered_map<string, Course>& courseMap);
void printCourseDetails(const unordered_map<string, Course>& courseMap);
void displayMenu();
void analyzeCoursePrerequisites(const unordered_map<string, Course>& courseMap);

int main() {
    // Store courses by identifier for average-case constant-time lookups.
    unordered_map<string, Course> courseMap;
    int choice;

    cout << "Welcome to the course planner." << endl;

    do {
        displayMenu();
        cout << endl;
        cout << "What would you like to do? ";

        // Read a numeric menu choice and recover from nonnumeric input.
        if (!(cin >> choice)) {
            if (cin.eof()) {
                cout << "\nInput closed. Exiting course planner." << endl;
                break;
            }

            // Clear the failed stream state and discard the invalid input line.
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input. Please enter a number from the menu."
                 << endl;
            continue;
        }

        // Dispatch the selected menu operation.
        switch (choice) {
        case 1: {
            string fileName;
            cout << "Enter the course data file name (Default is CourseInfo.csv): ";
            cin >> fileName;
            loadCoursesFromFile(fileName, courseMap);
            break;
        }
        case 2:
            printCourseList(courseMap);
            break;
        case 3:
            printCourseDetails(courseMap);
            break;
        case 4:
            analyzeCoursePrerequisites(courseMap);
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
        }

    } while (choice != 9);

    return 0;
}

// Prints the available menu choices.
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "4. Analyze Course Prerequisites." << endl;
    cout << "9. Exit" << endl;
}


// Reads course records from a comma-delimited file into courseMap.
void loadCoursesFromFile(
    const string& fileName,
    unordered_map<string, Course>& courseMap)
{
    ifstream file(fileName);

    // Leave existing course data unchanged if the file cannot be opened.
    if (!file.is_open()) {
        cout << "Error: The file cannot be opened." << endl;
        return;
    }

    string line;

    // Parse each input line as course number, title, then zero or more prerequisites.
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, title;

        // The first two comma-separated fields identify the course and its title.
        getline(ss, courseNumber, ',');
        getline(ss, title, ',');

        Course course;
        course.courseNumber = courseNumber;
        course.title = title;

        // Read and normalize the remaining fields as prerequisite identifiers.
        string prereq;
        while (getline(ss, prereq, ',')) {
            // Remove leading whitespace from the prerequisite field.
            prereq.erase(0, prereq.find_first_not_of(" \t\r\n"));

            // Remove trailing whitespace; handle a field containing only whitespace.
            size_t last = prereq.find_last_not_of(" \t\r\n");
            if (last == string::npos) {
                prereq.clear();
            }
            else {
                prereq.erase(last + 1);
            }

            // Ignore blank fields, but keep nonblank references even if their
            // course is absent so prerequisite analysis can report the error.
            if (!prereq.empty()) {
                course.prerequisites.push_back(prereq);
            }
        }

        // Insert or replace the record keyed by its course number.
        courseMap[courseNumber] = course;
    }

    file.close();
}

// Prints all loaded courses in course-number order.
void printCourseList(const unordered_map<string, Course>& courseMap) {
    vector<Course> courses;

    // Copy map values into a vector because an unordered_map has no sorted order.
    for (const auto& pair : courseMap) {
        courses.push_back(pair.second);
    }

    // Sort the copied records by their identifiers for consistent display.
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });

    cout << "Here is a sample schedule:" << endl << endl;

    for (const Course& course : courses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }
}

// Looks up a course identifier and prints its details if found.
void printCourseDetails(const unordered_map<string, Course>& courseMap) {
    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    // Normalize the identifier to uppercase before searching.
    transform(
        courseNumber.begin(),
        courseNumber.end(),
        courseNumber.begin(),
        ::toupper
    );

    auto it = courseMap.find(courseNumber);

    if (it != courseMap.end()) {
        it->second.printDetails();
    }
    else {
        cout << "Course not found." << endl;
    }
}


/*
 * Enhancement Two: Prerequisite Analysis
 *
 * Prompts for a course, traverses its full prerequisite dependency
 * graph, and prints a valid prerequisite-first sequence. Reports
 * missing references and cycles instead of displaying an invalid order.
 */
void analyzeCoursePrerequisites(
    const unordered_map<string, Course>& courseMap)
{
    // Analysis requires course records to have been loaded first.
    if (courseMap.empty()) {
        cout << "Please load course data before analyzing prerequisites."
             << endl;
        return;
    }

    string courseNumber;
    cout << "Enter the course number to analyze: ";
    cin >> courseNumber;

    // Match the case-insensitive lookup behavior used for course details.
    transform(
        courseNumber.begin(),
        courseNumber.end(),
        courseNumber.begin(),
        ::toupper
    );

    // Confirm that the requested starting course exists before building the graph.
    auto courseIt = courseMap.find(courseNumber);

    if (courseIt == courseMap.end()) {
        cout << "Course not found: " << courseNumber << endl;
        return;
    }

    // Build the prerequisite adjacency lists from the loaded course records.
    PrerequisiteGraph graph = buildPrerequisiteGraph(courseMap);

    // Maintain DFS state and collect a prerequisite-first output sequence.
    unordered_set<string> visited;
    unordered_set<string> active;
    vector<string> completionOrder;
    string errorMessage;

    // Traverse the requested course and all of its direct and indirect prerequisites.
    bool validSequence = depthFirstSearch(
        courseNumber,
        graph,
        visited,
        active,
        completionOrder,
        errorMessage
    );

    // Abort sequence output if DFS found a missing course or dependency cycle.
    if (!validSequence) {
        cout << "Unable to generate a valid course sequence." << endl;
        cout << "Error: " << errorMessage << endl;
        return;
    }

    // Display the requested course and every prerequisite in completion order.
    cout << endl;
    cout << "Prerequisite Analysis for "
         << courseIt->second.courseNumber
         << ", " << courseIt->second.title << endl;

    cout << "Recommended Course Completion Sequence:" << endl;

    for (size_t i = 0; i < completionOrder.size(); ++i) {
        // The graph stores identifiers; courseMap provides the display details.
        const Course& course = courseMap.at(completionOrder[i]);

        cout << (i + 1) << ". "
             << course.courseNumber << ", "
             << course.title << endl;
    }
}