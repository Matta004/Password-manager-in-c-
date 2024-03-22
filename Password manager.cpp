int main() {
    PasswordManager manager;
    string username, password;

    char choice;
    do {
        if (!manager.isLoggedInUser()) {
            cout << "Welcome to the Password Manager!" << endl;
            cout << "Do you want to:" << endl;
            cout << "1. Create a new user account" << endl;
            cout << "2. Log in" << endl;
            cout << "Enter your choice (1 or 2): ";
            cin >> choice;

            if (choice == '1') {

                // Creating a new user account
                cout << "Create an account:" << endl;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (manager.addUser(username, password)) {
                    cout << "Account created successfully!" << endl;
                }
                else {
                    cout << "Username already exists. Please choose a different username." << endl;
                }
            }
            else if (choice == '2') {
                // Logging in
                cout << "Login:" << endl;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (manager.login(username, password)) {
                    cout << "Login successful!" << endl;
                }
                else {
                    cout << "Invalid username or password." << endl;
                }
            }
            else {
                cout << "Invalid choice." << endl;
            }
        }
