/*
    Contact Management System (File-Based)
    ---------------------------------------
    Features:
      1. Add Contact
      2. View All Contacts
      3. Search Contact (by name or phone)
      4. Update Contact
      5. Delete Contact
      6. Exit

    Data is stored permanently in "contacts.txt" using a simple
    delimiter-based format so it can be read back and rewritten easily.

    Record format in file (one contact per line):
        id|name|phone|email|address
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

const string FILENAME = "contacts.txt";

struct Contact {
    int id;
    string name;
    string phone;
    string email;
    string address;
};

// ---------- Utility Functions ----------

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

// ---------- File Handling ----------

vector<Contact> loadContacts() {
    vector<Contact> contacts;
    ifstream file(FILENAME);
    if (!file.is_open()) return contacts;

    string line;
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        stringstream ss(line);
        Contact c;
        string idStr;

        getline(ss, idStr, '|');
        getline(ss, c.name, '|');
        getline(ss, c.phone, '|');
        getline(ss, c.email, '|');
        getline(ss, c.address, '|');

        try {
            c.id = stoi(idStr);
        } catch (...) {
            continue; // skip malformed line
        }
        contacts.push_back(c);
    }
    file.close();
    return contacts;
}

void saveContacts(const vector<Contact>& contacts) {
    ofstream file(FILENAME, ios::trunc);
    for (const auto& c : contacts) {
        file << c.id << "|" << c.name << "|" << c.phone << "|"
             << c.email << "|" << c.address << "\n";
    }
    file.close();
}

int getNextId(const vector<Contact>& contacts) {
    int maxId = 0;
    for (const auto& c : contacts)
        if (c.id > maxId) maxId = c.id;
    return maxId + 1;
}

// ---------- Core Features ----------

void addContact() {
    vector<Contact> contacts = loadContacts();
    Contact c;
    c.id = getNextId(contacts);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- Add New Contact ---\n";
    cout << "Name: ";
    getline(cin, c.name);
    cout << "Phone: ";
    getline(cin, c.phone);
    cout << "Email: ";
    getline(cin, c.email);
    cout << "Address: ";
    getline(cin, c.address);

    contacts.push_back(c);
    saveContacts(contacts);

    cout << "\nContact added successfully with ID: " << c.id << "\n";
}

void printContact(const Contact& c) {
    cout << "ID: " << c.id << "\n"
         << "Name: " << c.name << "\n"
         << "Phone: " << c.phone << "\n"
         << "Email: " << c.email << "\n"
         << "Address: " << c.address << "\n"
         << "----------------------------\n";
}

void viewContacts() {
    vector<Contact> contacts = loadContacts();
    cout << "\n--- All Contacts (" << contacts.size() << ") ---\n";

    if (contacts.empty()) {
        cout << "No contacts found.\n";
        return;
    }

    for (const auto& c : contacts)
        printContact(c);
}

void searchContact() {
    vector<Contact> contacts = loadContacts();
    if (contacts.empty()) {
        cout << "\nNo contacts to search.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nEnter name or phone number to search: ";
    string query;
    getline(cin, query);

    bool found = false;
    for (const auto& c : contacts) {
        if (c.name.find(query) != string::npos ||
            c.phone.find(query) != string::npos) {
            if (!found) cout << "\n--- Search Results ---\n";
            printContact(c);
            found = true;
        }
    }

    if (!found) cout << "\nNo matching contact found.\n";
}

void updateContact() {
    vector<Contact> contacts = loadContacts();
    if (contacts.empty()) {
        cout << "\nNo contacts available to update.\n";
        return;
    }

    cout << "\nEnter Contact ID to update: ";
    int id;
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool found = false;
    for (auto& c : contacts) {
        if (c.id == id) {
            found = true;
            cout << "Current details:\n";
            printContact(c);

            cout << "Enter new Name (leave blank to keep unchanged): ";
            string input;
            getline(cin, input);
            if (!trim(input).empty()) c.name = input;

            cout << "Enter new Phone (leave blank to keep unchanged): ";
            getline(cin, input);
            if (!trim(input).empty()) c.phone = input;

            cout << "Enter new Email (leave blank to keep unchanged): ";
            getline(cin, input);
            if (!trim(input).empty()) c.email = input;

            cout << "Enter new Address (leave blank to keep unchanged): ";
            getline(cin, input);
            if (!trim(input).empty()) c.address = input;

            break;
        }
    }

    if (found) {
        saveContacts(contacts);
        cout << "\nContact updated successfully.\n";
    } else {
        cout << "\nContact ID not found.\n";
    }
}

void deleteContact() {
    vector<Contact> contacts = loadContacts();
    if (contacts.empty()) {
        cout << "\nNo contacts available to delete.\n";
        return;
    }

    cout << "\nEnter Contact ID to delete: ";
    int id;
    cin >> id;

    bool found = false;
    vector<Contact> updated;
    for (const auto& c : contacts) {
        if (c.id == id) {
            found = true;
            continue; // skip, i.e., delete
        }
        updated.push_back(c);
    }

    if (found) {
        saveContacts(updated);
        cout << "\nContact deleted successfully.\n";
    } else {
        cout << "\nContact ID not found.\n";
    }
}

// ---------- Menu ----------

void showMenu() {
    cout << "\n============================\n";
    cout << "   CONTACT MANAGEMENT SYSTEM\n";
    cout << "============================\n";
    cout << "1. Add Contact\n";
    cout << "2. View All Contacts\n";
    cout << "3. Search Contact\n";
    cout << "4. Update Contact\n";
    cout << "5. Delete Contact\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    int choice;

    do {
        showMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: updateContact(); break;
            case 5: deleteContact(); break;
            case 6: cout << "\nExiting program. Goodbye!\n"; break;
            default: cout << "\nInvalid choice. Try again.\n";
        }

        if (choice != 6) pause();

    } while (choice != 6);

    return 0;
}