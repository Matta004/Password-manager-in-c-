# Password Manager Readme

## Description

This program is a simple command-line password manager implemented in C++. It allows users to create accounts, log in, store passwords for different websites, generate passwords, retrieve stored passwords, and log out.

## Features

1. **User Account Management:**
   - Users can create a new account by providing a unique username and password.
   - Account creation checks for duplicate usernames to ensure uniqueness.
   
2. **Login and Logout:**
   - Users can log in using their username and password.
   - Once logged in, users can perform various operations related to password management.
   - Users can log out to securely exit their session.

3. **Password Management:**
   - Users can add passwords for different websites along with corresponding usernames.
   - Passwords are encrypted before storage to enhance security.
   - The program allows users to generate strong passwords automatically.

4. **Password Retrieval:**
   - Users can retrieve stored passwords for specific websites.

5. **Stored Password Viewing:**
   - Users can view all stored passwords for their account.

## Usage

1. **Compiling:**
   - Compile the code using any C++ compiler.

2. **Running:**
   - Run the compiled executable file:
     ```
     ./password_manager
     ```

3. **Creating a New Account:**
   - Choose option 1 to create a new user account.
   - Enter a unique username and a password.

4. **Logging In:**
   - Choose option 2 to log in with an existing account.
   - Enter the username and password.

5. **Password Management:**
   - After logging in, choose from various options to manage passwords:
     - Option 1: Add a new password manually.
     - Option 2: Generate a new password automatically.
     - Option 3: Retrieve a password for a specific website.
     - Option 4: View all stored passwords.

6. **Logging Out:**
   - Choose option 5 to log out of the current session.

## Security Considerations

1. **Encryption:**
   - Passwords are encrypted using a Caesar cipher before storing them.
   - Decryption occurs only when passwords are retrieved, ensuring stored passwords remain secure.

2. **User Authentication:**
   - Users are required to log in with valid credentials before accessing their passwords.
   - Passwords are not visible without successful authentication.

3. **Password Generation:**
   - Users can generate strong passwords automatically, reducing the risk of using weak passwords.

## Note

- Ensure to keep the `users.txt` file and individual user password files securely.
- This password manager is for educational purposes and may not provide the same level of security as professional password managers.

## Author

- Youssef Matta

## Version

- V1.8

## Disclaimer

- The author is not responsible for any loss or damage resulting from the use of this software.
- Use at your own risk.
