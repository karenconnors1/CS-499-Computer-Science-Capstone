#include <iostream>
#include <string>
#include <limits>

using namespace std;

// SECURITY VULNERABILITY 1: The fixed-size username array
// can create a buffer overflow risk if input length is not
// properly limited when reading the username.
char username[20];

char name1[] = "Bob Jones";
char name2[] = "Sarah Davis";
char name3[] = "Amy Friendly";
char name4[] = "Johnny Smith";
char name5[] = "Carol Spears";

int num1 = 1;
int num2 = 2;
int num3 = 1;
int num4 = 1;
int num5 = 2;

int choice;
int changechoice;
int newservice;

// SECURITY VULNERABILITY 2: The password "123" is hardcoded
// and easily guessed. Storing credentials directly in the
// source code increases the risk of unauthorized access.
char password[20] = "123";

int answer;

int CheckUserPermissionAccess();
void DisplayInfo();
void ChangeCustomerChoice();

int main()
{
    cout << "Created by Karen Connors" << endl;
    cout << "Hello! Welcome to our Investment Company" << endl;

    // SECURITY FIX 6: Limit failed login attempts to five.
    // This reduces the risk of unlimited password guessing.
    int failedAttempts = 0;
    const int maxAttempts = 5;

    answer = CheckUserPermissionAccess();

    while (answer != 1 && failedAttempts < maxAttempts - 1)
    {
        failedAttempts++;
        cout << "Invalid Password. Please try again" << endl;
        answer = CheckUserPermissionAccess();
    }

    // SECURITY FIX 6: End the program after five failed attempts.
    if (answer != 1)
    {
        cout << "Maximum login attempts reached. Access denied." << endl;
        return 1;
    }

    do
    {
        cout << "What would you like to do?" << endl;
        cout << "DISPLAY the client list (enter 1)" << endl;
        cout << "CHANGE a client's choice (enter 2)" << endl;
        cout << "Exit the program.. (enter 3)" << endl;

        // SECURITY VULNERABILITY 4: The original menu did not validate
        // nonnumeric input or restrict selections to options 1, 2, and 3.

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

            cout << "Invalid input. Please enter a number from 1 to 3." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > 3)
        {
            cout << "Invalid choice. Please select 1, 2, or 3." << endl;
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

    } while (choice != 3);

    return 0;
}

int CheckUserPermissionAccess()
{
    string userPassword = "";
    int result = 0;

    cout << "Enter your username: " << endl;
    cin >> username;

    cout << "Enter your password: " << endl;
    cin >> userPassword;

    // SECURITY VULNERABILITY 3: The application accepts a username
    // but does not verify it against an authorized user account.
    // Access is granted based only on the password comparison.
    result = userPassword.compare("123");

    if (result == 0)
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
    cout << "  Client's Name    Service Selected (1 = Brokerage, 2 = Retirement)" << endl;

    cout << name1 << " selected option " << num1 << "\n";
    cout << name2 << " selected option " << num2 << "\n";
    cout << name3 << " selected option " << num3 << "\n";
    cout << name4 << " selected option " << num4 << "\n";
    cout << name5 << " selected option " << num5 << "\n";
}

void ChangeCustomerChoice()
{
    cout << "Enter the number of the client that you wish to change" << endl;

    // SECURITY VULNERABILITY 5: Client and service inputs are not
    // validated. Invalid client numbers, unsupported service values,
    // or nonnumeric entries can cause incorrect program behavior.
    cin >> changechoice;

    cout << "Please enter the client's new service choice (1 = Brokerage, 2 = Retirement)" << endl;
    cin >> newservice;

    // SECURITY VULNERABILITY 8: Client service changes are not logged.
    // The program does not record who made a change, when it occurred,
    // or the previous and updated service selections.
    if (changechoice == 1)
    {
        num1 = newservice;
    }
    else if (changechoice == 2)
    {
        num2 = newservice;
    }
    else if (changechoice == 3)
    {
        num3 = newservice;
    }
    else if (changechoice == 4)
    {
        num4 = newservice;
    }
    else if (changechoice == 5)
    {
        num5 = newservice;
    }
}
