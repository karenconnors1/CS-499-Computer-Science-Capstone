
/*
 * Project: SNHU Investments Client Management Application
 * File: Project2_Enhanced.cpp
 * Author: Karen Connors
 * Course: CS 499 - Computer Science Capstone
 * Original Artifact: CS 410 - Software Reverse Engineering
 * Enhancement Category: Software Design and Engineering
 * Date: November 13, 2026
 *
 * Purpose:
 * Enhance the original client management application to improve
 * software organization, functionality, reliability, and security.
 *
 * Planned Enhancements:
 * - Organize client information using structured records.
 * - Improve client management functionality.
 * - Strengthen input validation and error handling.
 * - Implement an audit history for client service changes.
 *
 * Course Outcomes:
 * Outcome 4 - Computing Practices and Software Engineering
 * Outcome 5 - Security Mindset
 */

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cctype> // Supports character validation and case-insensitive names.

using namespace std;

/*
 * Represents an investment client.
 * Groups the client's name and selected service
 * into a single record for easier maintenance.
 */
struct Client
{
    string name;
    int service;
};

/*
 * Represents an audit record for a client service change.
 * Stores the client name, previous service, new service,
 * and username responsible for the modification.
 */
struct AuditRecord
{
    string clientName;
    int previousService;
    int newService;
    string changedBy;
};

/*
 * Stores the investment clients in a single collection.
 * Each record contains the client's name and selected service.
 * Service 1 = Brokerage; Service 2 = Retirement.
 */
vector<Client> clients =
{
    {"Bob Jones", 1},
    {"Sarah Davis", 2},
    {"Amy Friendly", 1},
    {"Johnny Smith", 1},
    {"Carol Spears", 2}
};

/*
 * Stores the audit history of client service changes.
 * Each record identifies the client, previous service,
 * new service, and username responsible for the change.
 */

vector<AuditRecord> auditHistory;

/*
 * File used to store audit records between application sessions.
 */
const string AUDIT_FILE = "audit_history.txt";

/*
 * Stores newly added client records between application sessions.
 * Kept separate from the audit file to preserve existing audit history.
 */
const string ADDED_CLIENTS_FILE = "added_clients.txt";

/*
 * Stores the username using std::string instead of a
 * fixed-size character array to prevent buffer overflow
 * during username input.
 */
string username;


int choice;
int answer;

int CheckUserPermissionAccess();
void DisplayInfo();
/*
 * Declares the function for creating new investment clients.
 * Client information will be validated and saved to persistent
 * storage so newly added clients remain available after restart.
 */
void AddClient();
void ChangeCustomerChoice();
void DisplayAuditHistory();
bool SaveAuditRecord(const AuditRecord& record); 
bool SaveAddedClient(const Client& client);
void LoadAuditHistory();
void LoadAddedClients();

/*
 * Compares client names without regard to capitalization.
 * Prevents duplicate records such as "Bob Jones" and "bob jones"
 * while preserving the original spelling of stored client names.
 */
bool SameClientName(const string& left, const string& right)
{
    if (left.size() != right.size())
    {
        return false;
    }

    for (size_t i = 0; i < left.size(); ++i)
    {
        if (tolower(static_cast<unsigned char>(left[i])) !=
            tolower(static_cast<unsigned char>(right[i])))
        {
            return false;
        }
    }

    return true;
}

/*
 * Checks whether a client name already exists in the collection.
 * Uses case-insensitive comparison to prevent duplicate records.
 * This protects client data integrity when adding new clients.
 */
bool ClientExists(const string& name)
{
    for (const Client& client : clients)
    {
        if (SameClientName(client.name, name))
        {
            return true;
        }
    }

    return false;
}


/*
 * Validates a new client's name before it can be saved.
 * Accepts letters, spaces, hyphens, and apostrophes.
 * Rejects empty names, oversized names, and unsupported characters
 * to protect the integrity of stored client records.
 */
bool ValidClientName(const string& name)
{
    if (name.empty() || name.size() > 80)
    {
        return false;
    }

    // Prevent leading or trailing spaces.
    if (name.front() == ' ' || name.back() == ' ')
    {
        return false;
    }

    for (unsigned char character : name)
    {
        if (!isalpha(character) &&
            character != ' ' &&
            character != '-' &&
            character != '\'')
        {
            return false;
        }
    }

    return true;
}


int main()
{
    cout << "Created by Karen Connors" << endl;
    cout << "Hello! Welcome to our Investment Company" << endl;

    // SECURITY FIX 6: Limit failed login attempts to five.
    // This reduces the risk of unlimited password guessing.
    int failedAttempts = 0;
    const int maxAttempts = 5;

    answer = CheckUserPermissionAccess();

    // Exit if input ends during authentication.
    if (cin.eof())
    {
        cout << "Login input ended. Exiting program.\n";
        return 1;
    }

    while (answer != 1 && failedAttempts < maxAttempts - 1)
    {
        failedAttempts++;
        
        cout << "Invalid username or password. Please try again." << endl;

        answer = CheckUserPermissionAccess();

        // Stop retrying if input ends during authentication.
        if (cin.eof())
        {
            cout << "Login input ended. Exiting program.\n";
            return 1;
        }
    }

    // SECURITY FIX 6: End the program after five failed attempts.
    if (answer != 1)
    {
        cout << "Maximum login attempts reached. Access denied." << endl;
        return 1;
    }

    // Restore newly added clients before replaying saved service changes.
    // Loading in this order preserves updates to both original and added clients.
    LoadAddedClients();
    LoadAuditHistory();

    do
    {
        cout << "What would you like to do?" << endl;

        cout << "DISPLAY the client list (enter 1)" << endl;
        cout << "CHANGE a client's choice (enter 2)" << endl;
        cout << "VIEW the audit history (enter 3)" << endl;
        cout << "Exit the program (enter 4)" << endl;
        cout << "ADD a new client (enter 5)" << endl;


        // SECURITY VULNERABILITY 4: The original menu did not validate
        // nonnumeric input or restrict selections to options 1, 2, 3, and 4.

        // SECURITY FIX 4: Validate numeric input and permitted menu choices.
        // Clear invalid input to prevent the program from becoming stuck.
        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                cout << "Input ended. Exiting program." << endl;
                return 1;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input. Please enter a number from 1 to 5." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        
        // Accept menu options 1 through 5, including the new Add Client feature.
        if (choice < 1 || choice > 5)
        {
            cout << "Invalid choice. Please select 1, 2, 3, 4, or 5." << endl;
            continue;
        }


        cout << "You chose " << choice << endl;

        if (choice == 1)
        {
            DisplayInfo();
        }

        if (choice == 2)
        {
            ChangeCustomerChoice();
        }

        if (choice == 3)
        {
            DisplayAuditHistory();
        }

        if (choice == 4)
        {
            cout << "Exiting program." << endl;
        }   

        if (choice == 5)
        {
            AddClient();
        }
        
    } while (choice != 4);

    return 0;
}

int CheckUserPermissionAccess()
{
    string userPassword = "";

    // Read authorized credentials from environment variables.
    const char* authorizedUsername = std::getenv("INVESTMENT_USERNAME");
    const char* authorizedPassword = std::getenv("INVESTMENT_PASSWORD");

    cout << "Enter your username: " << endl;
    if (!(cin >> username))
    {
        return 0;
    }

    cout << "Enter your password: " << endl;
    if (!(cin >> userPassword))
    {
        return 0;
    }   

    // Deny access if authorized credentials are not configured.
    if (authorizedUsername == nullptr ||
        authorizedPassword == nullptr)
    {
        cout << "Authentication is not configured.\n";
        return 2;
    }

    // Verify both the username and password.
    if (username == authorizedUsername &&
        userPassword == authorizedPassword)
    {
        return 1;
    }
    else
    {
        return 2;
    }

}

// SECURITY VULNERABILITY 7: Client-level authorization is missing.
// Any authenticated user can view all client records without
// verifying whether they have permission to access those records.

void DisplayInfo()

{
    cout << "\nClient List\n";
    cout << "Service Selected (1 = Brokerage, 2 = Retirement)\n";

    // Display each client from the structured collection.
    for (size_t i = 0; i < clients.size(); ++i)
    {
        cout << (i + 1) << ". "
             << clients[i].name
             << " selected option "
             << clients[i].service
             << "\n";
    }
}


/*
 * Adds a new client after validating the name and service.
 * Prevents duplicate names and saves the record before adding
 * it to the in-memory client collection.
 */
void AddClient()
{
    string clientName;
    int service;

    cout << "\nEnter the new client's full name: ";

    if (!getline(cin, clientName))
    {
        cout << "Unable to read client name.\n";
        return;
    }

    // Reject names that do not meet validation requirements.
    if (!ValidClientName(clientName))
    {
        cout << "Invalid name. Use 1-80 characters: letters, spaces, hyphens, or apostrophes.\n";
        return;
    }

    // Do not permit an existing client to be entered twice.
    if (ClientExists(clientName))
    {
        cout << "A client with this name already exists.\n";
        return;
    }

    cout << "Enter service (1 = Brokerage, 2 = Retirement): ";

    if (!(cin >> service))
    {
        if (!cin.eof())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please enter 1 or 2.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (service != 1 && service != 2)
    {
        cout << "Invalid service choice. Please enter 1 or 2.\n";
        return;
    }

    Client newClient = {clientName, service};

    // Save first so a failed write does not create an unsaved client.
    if (!SaveAddedClient(newClient))
    {
        cerr << "Client was not added because saving failed.\n";
        return;
    }

    clients.push_back(newClient);

    cout << "New client added successfully.\n";
}


void ChangeCustomerChoice()
{
    int clientNumber;
    int newService;

    cout << "\nEnter the number of the client you wish to change (1-"
         << clients.size() << "): ";

    // Verify that a valid client number was entered.
    if (!(cin >> clientNumber))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a numeric client number.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Ensure the selected client exists in the collection.
    if (clientNumber < 1 ||
        static_cast<size_t>(clientNumber) > clients.size())
    {
        cout << "Invalid client number.\n";
        return;
    }

    cout << "Enter the new service (1 = Brokerage, 2 = Retirement): ";

    // Verify that a numeric service choice was entered.
    if (!(cin >> newService))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a numeric service choice.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Accept only supported investment service options.
    if (newService != 1 && newService != 2)
    {
        cout << "Invalid service choice. Please enter 1 or 2.\n";
        return;
    }
    
    // Save the previous service for audit history.
    int previousService = clients[clientNumber - 1].service;

    // Prevent recording a change when the service is unchanged.
    if (previousService == newService)
    {
        cout << "The client already has this service. No change made.\n";
        return;
    }

    // Record the service change in the audit history.
    auditHistory.push_back({
        clients[clientNumber - 1].name,
        previousService,
        newService,
        username
    });
    
    if (!SaveAuditRecord(auditHistory.back()))
    {
        // Remove the unsaved audit record from memory.
        auditHistory.pop_back();

        cerr << "Error: Client service was not changed because the audit record could not be saved.\n";
        return;
    }

    // Update the client only after the audit file is saved.
    clients[clientNumber - 1].service = newService;

    cout << "Client service updated successfully.\n";

}

/*
 * Displays the audit history of client service changes.
 * Shows the client, previous service, new service,
 * and username responsible for each change.
 */
void DisplayAuditHistory()
{
    cout << "\nClient Service Change Audit History\n";

    if (auditHistory.empty())
    {
        cout << "No service changes have been recorded.\n";
        return;
    }

    for (size_t i = 0; i < auditHistory.size(); ++i)
    {
        cout << "\nAudit Record " << (i + 1) << "\n";
        cout << "Client: " << auditHistory[i].clientName << "\n";
        cout << "Previous Service: " << auditHistory[i].previousService << "\n";
        cout << "New Service: " << auditHistory[i].newService << "\n";
        cout << "Changed By: " << auditHistory[i].changedBy << "\n";
    }
}

/*
 * Appends a client service change to the audit history file.
 * Reports an error if the file cannot be written.
 */
bool SaveAuditRecord(const AuditRecord& record)
{
    ofstream auditFile(AUDIT_FILE, ios::app);

    if (!auditFile.is_open())
    {
        cerr << "Error: Unable to open audit history file.\n";
        return false;
    }

    auditFile << record.clientName << "|"
              << record.previousService << "|"
              << record.newService << "|"
              << record.changedBy << "\n";

    auditFile.close();

    
    // Check whether writing or closing the audit file failed.
    if (auditFile.fail())
    {
        cerr << "Error: Audit record may not have been saved.\n";
        return false;
    }
    return true;
}

/*
 * Loads previously saved audit records from the audit file.
 * A missing file is expected when the application first runs.
 */

void LoadAuditHistory()
{
    ifstream auditFile(AUDIT_FILE);

    if (!auditFile.is_open())
    {
        return;
    }

    // Read each saved audit record one line at a time.
    string line;

    while (getline(auditFile, line))
    {
        // Prepare the saved line for field-by-field parsing.
        stringstream recordStream(line);

        // Store the four fields from the saved audit record.

        string clientName;
        string previousServiceText;
        string newServiceText;
        string changedBy;

        // Store validated numeric service values.
        int previousService;
        int newService;


        // Extract the four fields separated by "|" characters.
        if (!getline(recordStream, clientName, '|') ||
            !getline(recordStream, previousServiceText, '|') ||
            !getline(recordStream, newServiceText, '|') ||
            !getline(recordStream, changedBy))
        {
            // Skip incomplete audit records.
            continue;
        }
    
        
        // Validate service values before converting them.
        if ((previousServiceText != "1" && previousServiceText != "2") ||
            (newServiceText != "1" && newServiceText != "2"))
        {
            // Skip records containing invalid service values.
            continue;
        }

        previousService = stoi(previousServiceText);
        newService = stoi(newServiceText);

        // Restore the validated record to the audit history.
        auditHistory.push_back({
            clientName,
            previousService,
            newService,
            changedBy
        });

        // Restore the client's most recently recorded service.
        for (Client& client : clients)
        {
            if (client.name == clientName)
            {
                client.service = newService;
                break;
            }
        }
    }

    auditFile.close();
}

/*
 * Saves a newly created client to persistent storage.
 * Each record contains the client's name and selected service.
 *
 * The file is opened in append mode to preserve existing clients.
 * Returns false if the file cannot be opened or written successfully.
 * The calling function must not add the client to memory if saving fails.
 */
bool SaveAddedClient(const Client& client)
{
    ofstream clientFile(ADDED_CLIENTS_FILE, ios::app);

    // Prevent saving if the client data file cannot be opened.
    if (!clientFile.is_open())
    {
        cerr << "Error: Unable to open added clients file.\n";
        return false;
    }

    // Store client data using the same simple delimiter format.
    clientFile << client.name << "|"
               << client.service << "\n";

    clientFile.close();

    // Verify that writing and closing the file succeeded.
    if (clientFile.fail())
    {
        cerr << "Error: New client record may not have been saved.\n";
        return false;
    }

    return true;
}


/*
 * Loads previously added clients from persistent storage.
 * Each saved record contains the client's name and service.
 *
 * A missing file is normal when no clients have been added.
 * Invalid records and duplicate names are skipped to protect
 * the original client collection and maintain data integrity.
 */
void LoadAddedClients()
{
    ifstream clientFile(ADDED_CLIENTS_FILE);

    if (!clientFile.is_open())
    {
        return;
    }

    string line;

    // Process saved clients one record at a time.
    while (getline(clientFile, line))
    {
        stringstream recordStream(line);
        string clientName;
        string serviceText;

        // Require both fields in the saved client record.
        if (!getline(recordStream, clientName, '|') ||
            !getline(recordStream, serviceText))
        {
            continue;
        }

        // Reject empty, duplicate, or malformed client records.
        if (clientName.empty() || clientName.find('|') != string::npos ||
            ClientExists(clientName) ||
            (serviceText != "1" && serviceText != "2"))
        {
            continue;
        }

        // Restore the client while preserving the saved name.
        int service = serviceText[0] - '0';
        clients.push_back({clientName, service});
    }

    clientFile.close();
}
