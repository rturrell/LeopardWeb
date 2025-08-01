#include <iostream>
#include <sqlite3.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <tuple>
#include <vector>
#include <conio.h> // For _getch() on Windows
#include "myfunc.h"

using namespace std;

// Masked password input
string getMaskedPassword() {
    string password;
    char ch;

    cout << "Enter password: ";
    while (true) {
        ch = _getch();

        if (ch == 13) { // Enter key
            break;
        }
        else if (ch == 8) { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

int main(int argc, char** argv) {
    sqlite3* DB;
    int exit = sqlite3_open("assignment3.db", &DB);
    if (exit != SQLITE_OK) {
        cerr << "Error opening database\n";
        return -1;
    }

    // Initialize database tables
    createStudentTable(DB);
    recreateInstructorTable(DB);
    recreateAdministratorTable(DB);
    createCourseTable(DB);
    createLoginTable(DB);
    createSScheduleTable(DB);
    createCourseRosterTable(DB);
    insertInitialData(DB);

    // === Login Loop ===
    string username, password, roless;

    do {
        cout << "Enter username: ";
        cin >> username;
        password = getMaskedPassword();

        roless = checklogin(DB, username, password);

        if (roless.empty()) {
            cout << "Please try again.\n\n";
        }

    } while (roless.empty());

    // === Get ID after login ===
    string idStr;
    int id = -1;
    if (roless == "STUDENT") {
        string queryID = "SELECT ID FROM STUDENT WHERE EMAIL = '" + username + "';";
        sqlite3_exec(DB, queryID.c_str(), lcallback, &idStr, nullptr);
        id = stoi(idStr);
    }
    else if (roless == "INSTRUCTOR") {
        string queryID = "SELECT ID FROM INSTRUCTOR WHERE EMAIL = '" + username + "';";
        sqlite3_exec(DB, queryID.c_str(), lcallback, &idStr, nullptr);
        id = stoi(idStr);
    }

    // === Role Menus ===
    if (roless == "STUDENT") {
        int choice;
        do {
            cout << "\nMenu:\n"
                << "1. Search Courses by ID\n"
                << "2. Search courses by CRN\n"
                << "3. Add course to schedule\n"
                << "4. Remove course from schedule\n"
                << "5. Print schedule\n"
                << "6. Exit\n"
                << "Enter choice\n";
            cin >> choice;
            switch (choice) {
            case 1: searchCoursesByID(DB); break;
            case 2: searchCourseByCRN(DB); break;
            case 3: addCourseToSchedule(DB, id); break;
            case 4: removeCourseFromSchedule(DB, id); break;
            case 5: printStudentSchedule(DB, id); break;
            case 6: cout << "Exiting" << endl; break;
            default: cout << "Invalid choice" << endl; break;
            }
        } while (choice != 6);
    }
    else if (roless == "INSTRUCTOR") {
        int choice;
        do {
            cout << "\nMenu:\n"
                << "1. Search courses by ID\n"
                << "2. Search courses by CRN\n"
                << "3. Print schedule\n"
                << "4. Search course roster\n"
                << "5. Print roster\n"
                << "6. Exit\n"
                << "Enter choice\n";
            cin >> choice;
            switch (choice) {
            case 1: searchCoursesByID(DB); break;
            case 2: searchCourseByCRN(DB); break;
            case 3: printInstructorSchedule(DB, id); break;
            case 4: searchCourseRoster(DB, id); break;
            case 5: printRoster(DB, id); break;
            case 6: cout << "Exiting" << endl; break;
            default: cout << "Invalid choice" << endl; break;
            }
        } while (choice != 6);
    }
    else if (roless == "ADMINISTRATOR") {
        int choice;
        do {
            cout << "\nMenu:\n"
                << "1. Search courses by ID\n"
                << "2. Search courses by CRN\n"
                << "3. Add courses to system\n"
                << "4. Remove courses from system\n"
                << "5. Add student to system\n"
                << "6. Add instructor to system\n"
                << "7. Link instructor to course\n"
                << "8. Unlink instructor from course\n"
                << "9. Add a student to a course\n"
                << "10. Remove student from course\n"
                << "11. Exit\n"
                << "Enter choice\n";
            cin >> choice;
            switch (choice) {
            case 1: searchCourseByID(DB); break;
            case 2: searchCourseByCRN(DB); break;
            case 3: addCourseToSystem(DB); break;
            case 4: removeCourseFromSystem(DB); break;
            case 5: addStudentToSystem(DB); break;
            case 6: addInstructorToSystem(DB); break;
            case 7: linkInstructorToCourse(DB); break;
            case 8: unlinkInstructorFromCourse(DB); break;
            case 9: addStudentToCourse(DB); break;
            case 10: removeStudentFromCourse(DB); break;
            case 11: cout << "Exiting" << endl; break;
            default: cout << "Invalid choice" << endl; break;
            }
        } while (choice != 11);
    }

    sqlite3_close(DB);
    return 0;
}
