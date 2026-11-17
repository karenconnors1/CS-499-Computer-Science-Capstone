
# Investment Client Management System
## SNHU CS 499 – Enhancement One: Software Design and Engineering

### Project Overview
This enhanced C++ application manages investment clients and their selected financial services. The application emphasizes secure authentication, structured client records, input validation, persistent data storage, and audit tracking.

The application allows an authenticated user to:
- Display the client list and selected financial services.
- Add new clients with validated names and service selections.
- Prevent duplicate client records using case-insensitive name comparison.
- Change an existing client's selected service.
- View the client service change audit history.
- Restore saved clients and service changes across application sessions.
- Exit the program.

### Software Design and Engineering Enhancements
The enhanced application includes:
- Structured client records using a C++ `Client` structure and `vector` collection.
- Authentication using environment variables instead of hardcoded credentials.
- A maximum of five failed login attempts.
- Input validation for menu choices, client numbers, and service selections.
- A new Add Client feature for creating investment client records.
- Client-name validation that rejects empty names, excessive lengths, and unsupported characters.
- Case-insensitive duplicate-name detection to protect client data integrity.
- Persistent storage of newly added clients in `added_clients.txt`.
- Automatic restoration of saved clients when the application restarts.
- Persistent service-change audit history stored in `audit_history.txt`.
- Restoration of recorded client service changes after loading saved clients.
- Prevention of unnecessary audit records when a service remains unchanged.
- Error handling that prevents client updates when an audit record cannot be saved.
- Error handling that prevents new clients from being added to memory when saving fails.

### Files
- `Project2_Enhanced.cpp` – Enhanced C++ source code containing the client management, validation, authentication, and audit-history functionality.
- `added_clients.txt` – Persistent records for newly created investment clients.
- `audit_history.txt` – Saved client service changes, including the previous service, new service, and username responsible.
- `Project2_Enhanced` – Compiled macOS executable.

### Compilation
Compile the source code using a C++17-compatible compiler:

```bash
clang++ -std=c++17 -Wall -Wextra -pedantic Project2_Enhanced.cpp -o Project2_Enhanced
```

### Running the Application
Configure the `INVESTMENT_USERNAME` and `INVESTMENT_PASSWORD` environment variables before launching the application.

Run the compiled program from the directory containing the audit history file:

```bash
./Project2_Enhanced
```

### Testing
Testing included:
- Successful compilation using C++17 with additional compiler warnings enabled.
- Successful authentication and application startup.
- Rejection of invalid login credentials.
- Handling of missing login input.
- Displaying the original investment client records.
- Successfully adding a new client with a valid name and service selection.
- Confirming that newly added clients appear in the client list.
- Verifying that newly added clients remain available after restarting the application.
- Rejecting duplicate client names regardless of capitalization.
- Rejecting client names containing unsupported characters.
- Rejecting invalid service selections when adding clients.
- Successfully changing the service of a newly added client.
- Recording the new client's service change in the audit history.
- Verifying that updated client services persist after restarting the application.
- Verifying that audit records persist after restarting the application.
- Preventing unnecessary audit records when a service remains unchanged.
- Handling an audit-file write failure without modifying client data.

### Security Note
The authentication mechanism is intended for educational demonstration. Production financial software would require stronger authentication, secure credential storage, access controls, and additional security protections.
