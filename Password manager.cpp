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

    int passwordCount;

    string loggedInUser; // Store the username of the logged-in user
    bool isLoggedIn;     // Track login status

    const int shift = 3; // Caesar cipher shift value

public:
    PasswordManager() : userCount(0), passwordCount(0), isLoggedIn(false) {
        loadUsers();
    }

    bool addUser(const string& username, const string& password) {
        for (int i = 0; i < userCount; ++i) {
            if (users[i].getUsername() == username) {
                return false; // User already exists
            }
        }
        if (userCount < MAX_USERS) {
            users[userCount++] = User(encrypt(username), password); // Caesar ciphering the username here
            saveUsers(); // Save the new user
            return true;
        }
        return false; // Maximum users reached
    }

    bool login(const string& username, const string& password) {
        for (int i = 0; i < userCount; ++i) {
            if (users[i].getUsername() == encrypt(username) && users[i].getPassword() == password) {
                loggedInUser = username; // Store the logged-in user
                isLoggedIn = true;       // Update login status
                loadPasswords(); // Load passwords for the logged-in user
                return true;             // Login successful
            }
        }
        return false; // Invalid username or password
    }

    void logout() {
        loggedInUser = ""; // Clear the logged-in user
        isLoggedIn = false; // Update login status
    }

    void addPassword(const string& website, const string& username, const string& password) {
        if (passwordCount < MAX_PASSWORDS) {
            ofstream file(loggedInUser + ".txt", ios::app); // Open the user's file in append mode
            if (file.is_open()) {
                file << encrypt(website) << " " << encrypt(username) << " " << encrypt(password) << endl;
                file.close();
                passwordCount++;
            }
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

    void printStoredPasswords() {
        ifstream file(loggedInUser + ".txt");
        if (file.is_open()) {
            string site, user, pass;
            bool hasPasswords = false; // Flag to track if any passwords are stored
            cout << "Stored passwords:" << endl;
            while (file >> site >> user >> pass) {
                // Decrypt website, username, and password before printing
                cout << "Website: " << decrypt(site) << ", Username: " << decrypt(user) << ", Password: " << decrypt(pass) << endl;
                hasPasswords = true;
            }
            file.close();

            if (!hasPasswords) {
                cout << "No passwords stored for the current user." << endl;
            }
        }
        else {
            cout << "No passwords stored for the current user." << endl;
        }
    }
    bool getPassword(const string& website, const string& username, string& retrievedPassword) {
        ifstream file(loggedInUser + ".txt");
        if (file.is_open()) {
            string site, user, encryptedPass;
            while (file >> site >> user >> encryptedPass) {
                if (decrypt(site) == website && decrypt(user) == username) {
                    retrievedPassword = decrypt(encryptedPass); // Decrypt and return the password
                    cout << "Password retrieval successful." << endl;
                    file.close();
                    return true;
                }
            }
            file.close();
        }
        return false; // Password not found for the given website and username
    }



    bool isLoggedInUser() const {
        return isLoggedIn;
    }

    const string& getLoggedInUser() const {
        return loggedInUser;
    }

    string encrypt(const string& input) const {
        string result = input;
        for (char& c : result) {
            if (isalpha(c)) {
                char base = islower(static_cast<int>(c)) ? 'a' : 'A'; // Determine the base value for shifting
                c = base + (c - base + shift) % 26; // Shift character
            }
            else if (isdigit(c)) {
                c = '0' + (c - '0' + shift) % 10; // Shift digit
            }
        }
        return result;
    }

    string decrypt(const string& input) const {
        string result = input;
        for (char& c : result) {
            if (isalpha(c)) {
                char base = islower(static_cast<int>(c)) ? 'a' : 'A'; // Determine the base value for shifting
                c = base + (c - base - shift + 26) % 26; // Shift character back
            }
            else if (isdigit(c)) {
                c = '0' + (c - '0' - shift + 10) % 10; // Shift digit back
            }
        }
        return result;
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
        ifstream file(loggedInUser + ".txt");
        if (file.is_open()) {
            string site, user, pass;
            while (file >> site >> user >> pass) {
                passwordCount++;
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
            cout << "Welcome to Password Manager!" << endl;
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
            cout << "2. Generate a new password" << endl;
            cout << "3. Retrieve a password" << endl;
            cout << "4. View stored passwords" << endl;
            cout << "5. Log out" << endl;
            cout << "Enter your choice (1, 2, 3, 4, or 5): ";
            cin >> choice;

            if (choice == '1') {
                string website, newPassword;
                cout << "Enter website: ";
                cin >> website;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> newPassword;
                manager.addPassword(website, username, newPassword);
                cout << "Password added successfully!" << endl;
            }
            else if (choice == '2') {
                string website, username, newPassword;
                cout << "Enter website: ";
                cin >> website;
                cout << "Enter username: ";
                cin >> username;
                manager.generatePassword(newPassword);
                cout << "Generated Password: " << newPassword << endl;
                manager.addPassword(website, username, newPassword);
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
                // Print stored passwords
                manager.printStoredPasswords();
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
