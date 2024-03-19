#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class User {
private:
    string username;
    string password;

public:
    User() : username(""), password("") {}
    User(const string& _username, const string& _password) : username(_username), password(_password) {}

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }
};

class PasswordManager {
private:
    static const int MAX_USERS = 100;
    static const int MAX_PASSWORDS = 100;

    User users[MAX_USERS];
    int userCount;

    pair<string, string> passwords[MAX_PASSWORDS];
    int passwordCount;

    string loggedInUser; // Store the username of the logged-in user
    bool isLoggedIn;     // Track login status

public:
    PasswordManager() : userCount(0), passwordCount(0), isLoggedIn(false) {
        loadUsers();
        loadPasswords();
    }

    bool addUser(const string& username, const string& password) {
        for (int i = 0; i < userCount; ++i) {
            if (users[i].getUsername() == username) {
                return false; // User already exists
            }
        }
        if (userCount < MAX_USERS) {
            users[userCount++] = User(username, password);
            saveUsers(); // Save the new user
            return true;
        }
        return false; // Maximum users reached
    }

    bool login(const string& username, const string& password) {
        for (int i = 0; i < userCount; ++i) {
            if (users[i].getUsername() == username && users[i].getPassword() == password) {
                loggedInUser = username; // Store the logged-in user
                isLoggedIn = true;       // Update login status
                return true;             // Login successful
            }
        }
        return false; // Invalid username or password
    }

    void logout() {
        loggedInUser = ""; // Clear the logged-in user
        isLoggedIn = false; // Update login status
    }

    void addPassword(const string& website, const string& password, const string& username) {
        if (passwordCount < MAX_PASSWORDS) {
            passwords[passwordCount++] = make_pair(username + "_" + website, password);
            savePasswords(); // Save the new password
        }
    }

    void generatePassword(string& password, int length = 12) {
        static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{};:,.<>?";
        static const int charsetSize = sizeof(charset) - 1;
        srand(time(0)); // Seed the random number generator
        password.clear();
        for (int i = 0; i < length; ++i) {
            password += charset[rand() % charsetSize];
        }
    }

    bool getPassword(const string& website, const string& username, string& password) {
        for (int i = 0; i < passwordCount; ++i) {
            if (passwords[i].first == username + "_" + website) {
                password = passwords[i].second;
                return true;
            }
        }
        return false; // Password not found
    }

    void printAllPasswords(const string& username) {
        bool found = false;
        for (int i = 0; i < passwordCount; ++i) {
            size_t pos = passwords[i].first.find('_');
            string entryUsername = passwords[i].first.substr(0, pos);
            if (entryUsername == username) {
                if (!found) {
                    cout << "Stored Passwords for " << username << ":" << endl;
                    found = true;
                }
                string website = passwords[i].first.substr(pos + 1);
                cout << "Website: " << website << ", Password: " << passwords[i].second << endl;
            }
        }
        if (!found) {
            cout << "No passwords stored." << endl;
        }
    }

    bool isLoggedInUser() const {
        return isLoggedIn;
    }

    const string& getLoggedInUser() const {
        return loggedInUser;
    }

private:
    void loadUsers() {
        ifstream file("users.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line) && userCount < MAX_USERS) {
                stringstream ss(line);
                string username, password;
                ss >> username >> password;
                users[userCount++] = User(username, password);
            }
            file.close();
        }
    }

    void saveUsers() {
        ofstream file("users.txt");
        if (file.is_open()) {
            for (int i = 0; i < userCount; ++i) {
                file << users[i].getUsername() << " " << users[i].getPassword() << endl;
            }
            file.close();
        }
    }

    void loadPasswords() {
        ifstream file("passwords.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line) && passwordCount < MAX_PASSWORDS) {
                stringstream ss(line);
                string username_website, password;
                ss >> username_website >> password;
                passwords[passwordCount++] = make_pair(username_website, password);
            }
            file.close();
        }
    }

    void savePasswords() {
        ofstream file("passwords.txt");
        if (file.is_open()) {
            for (int i = 0; i < passwordCount; ++i) {
                file << passwords[i].first << " " << passwords[i].second << endl;
            }
            file.close();
        }
    }
};

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

        if (manager.isLoggedInUser()) {
            // After successful login, present options to the user
            cout << "What do you want to do?" << endl;
            cout << "1. Add a new password" << endl;
            cout << "2. Add a new password with Generat a random password" << endl;
            cout << "3. Retrieve a password" << endl;
            cout << "4. View stored passwords" << endl;
            cout << "5. Log out" << endl;
            cout << "Enter your choice (1, 2, 3, 4, or 5): ";
            cin >> choice;

            if (choice == '1') {
                string website, newPassword;
                cout << "Enter website: ";
                cin >> website;
                cout << "Enter password: ";
                cin >> newPassword;
                manager.addPassword(website, newPassword, username);
                cout << "Password added successfully!" << endl;
            }
            else if (choice == '2') {
                string website;
                cout << "Enter website: ";
                cin >> website;
                string newPassword;
                manager.generatePassword(newPassword);
                cout << "Generated Password: " << newPassword << endl;
                manager.addPassword(website, newPassword, username);
                cout << "Password added successfully!" << endl;
            }
            else if (choice == '3') {
                string website, retrievedPassword;
                cout << "Enter website: ";
                cin >> website;
                if (manager.getPassword(website, username, retrievedPassword)) {
                    cout << "Retrieved Password: " << retrievedPassword << endl;
                }
                else {
                    cout << "No password found for this website." << endl;
                }
            }
            else if (choice == '4') {
                manager.printAllPasswords(username);
            }
            else if (choice == '5') {
                manager.logout();
                cout << "Logged out successfully!" << endl;
            }
            else {
                cout << "Invalid choice." << endl;
            }
        }

        cout << "Do you want to continue? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Exiting Password Manager. Goodbye!" << endl;

    return 0;
}