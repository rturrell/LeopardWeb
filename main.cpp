#include <iostream>
#include <sqlite3.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <tuple>
#include<vector>
using std::vector;

using namespace std;


static int callback(void* data, int argc, char** argv, char** azColName) { //Ergisa 
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void printCoursesWithInstructors(sqlite3* DB) { //Isabella
    string sql = R"(
        SELECT 
            C.CRN,
            C.TITLE,
            C.DEPARTMENT,
            C.STARTTIME,
            C.ENDTIME,
            C.DAY1,
            C.DAY2,
            C.SEMESTER,
            C.YEAR,
            C.CREDITS,
            IFNULL(I.FIRSTNAME || ' ' || I.SURNAME, 'No matching instructor') AS Instructor
        FROM COURSE C
        LEFT JOIN INSTRUCTOR I ON C.DEPARTMENT = I.DEPARTMENT
        ORDER BY C.TITLE;
    )";

    auto callback = [](void*, int argc, char** argv, char**) -> int {
        cout << "\nCourse:\n";
        cout << "CRN: " << argv[0]
            << ", Title: " << argv[1]
            << ", Department: " << argv[2]
            << ", Time: " << argv[3] << "-" << argv[4]
            << ", Days: " << argv[5] << "/" << argv[6]
            << ", Semester: " << argv[7]
            << ", Year: " << argv[8]
            << ", Credits: " << argv[9]
            << ", Instructor: " << argv[10] << endl;
        return 0;
        };

    int rc = sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error retrieving courses: " << sqlite3_errmsg(DB) << endl;
    }
}

void searchCoursesByID(sqlite3* DB) { //Dylan
    string id;
    cout << "Enter course ID (e.g., ELEC 3000): ";
    cin.ignore();
    getline(cin, id);

    string sql = "SELECT * FROM COURSE WHERE ID = '" + id + "';";
    auto callback = [](void*, int argc, char** argv, char** col) -> int {
        for (int i = 0; i < argc; i++) {
            cout << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "------------------\n";
        return 0;
        };
    if (sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Error searching courses by ID.\n";
    }
}
void printRoster(sqlite3* DB, int instructorID) { //Dylan
    int crn;
    cout << "Enter CRN of the course: ";
    cin >> crn;

    string sql = "SELECT STUDENT.ID, STUDENT.NAME "
        "FROM STUDENT "
        "JOIN SSCHEDULE ON STUDENT.ID = SSCHEDULE.ID "
        "WHERE SSCHEDULE.CRN = " + to_string(crn) + ";";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        cout << "Student ID: " << argv[0] << ", Name: " << argv[1] << endl;
        return 0;
        };

    int rc = sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error retrieving roster: " << sqlite3_errmsg(DB) << endl;
    }
}
void printInstructorSchedule(sqlite3* DB, int instructorID) { //Ergisa
    string sql = "SELECT CRN, ID, TITLE, STARTTIME, ENDTIME, DAY1, DAY2, SEMESTER, YEAR "
        "FROM COURSE WHERE INSTRUCTOR = " + to_string(instructorID) + ";";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        cout << "\nCRN: " << argv[0]
            << ", Course ID: " << argv[1]
            << ", Title: " << argv[2]
            << ", Time: " << argv[3] << "-" << argv[4]
            << ", Days: " << argv[5] << "/" << argv[6]
            << ", Semester: " << argv[7]
            << ", Year: " << argv[8] << endl;
        return 0;
        };

    int rc = sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error retrieving instructor schedule: " << sqlite3_errmsg(DB) << endl;
    }
}


void searchCourseByCRN(sqlite3* DB) {
    int crn;
    cout << "Enter CRN (e.g., 121): ";
    cin >> crn;

    string sql = "SELECT * FROM COURSE WHERE CRN = " + to_string(crn) + ";";

    auto callback = [](void*, int argc, char** argv, char** col) -> int {
        for (int i = 0; i < argc; i++) {
            cout << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "------------------\n";
        return 0;
        };

    if (sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Error searching course by CRN.\n";
    }
}

void removeCourseFromSchedule(sqlite3* DB, int studentID) { //Isabella
    string courseID;
    cout << "Enter Course ID to remove: ";
    cin >> ws;
    getline(cin, courseID);

    string sql = "DELETE FROM SSCHEDULE WHERE ID = " + to_string(studentID) +
        " AND COURSEID = '" + courseID + "';";

    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error removing course: " << sqlite3_errmsg(DB) << endl;
    }
    else {
        cout << "Course removed from schedule successfully.\n";
    }
}

void searchCourseByID(sqlite3* DB) { //Ergisa 
    string id;
    cout << "Enter course ID (e.g., ELEC 3200): ";
    cin.ignore();
    getline(cin, id);

    string sql = "SELECT * FROM COURSE WHERE ID = '" + id + "';";
    auto callback = [](void*, int argc, char** argv, char** colName) -> int {
        for (int i = 0; i < argc; i++) {
            cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
        }
        cout << endl;
        return 0;
        };

    int rc = sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error: " << sqlite3_errmsg(DB) << endl;
    }
}

void searchCourseByDay(sqlite3* DB) { //Ergisa
    string day;
    cout << "Enter day (e.g., Monday): ";
    cin.ignore();
    getline(cin, day);

    string sql = "SELECT * FROM COURSE WHERE DAY = '" + day + "';";
    auto callback = [](void*, int argc, char** argv, char** colName) -> int {
        for (int i = 0; i < argc; i++) {
            cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
        }
        cout << endl;
        return 0;
        };

    int rc = sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error: " << sqlite3_errmsg(DB) << endl;
    }
}



void addStudentToSystem(sqlite3* DB) {
    int id, gradYear;
    string fname, lname, major, email;

    cout << "Enter student ID: ";
    cin >> id;
    cin.ignore(); // clear input buffer

    cout << "Enter first name: ";
    getline(cin, fname);

    cout << "Enter last name: ";
    getline(cin, lname);

    cout << "Enter expected graduation year: ";
    cin >> gradYear;
    cin.ignore();

    cout << "Enter major: ";
    getline(cin, major);

    cout << "Enter email: ";
    getline(cin, email);

    string sql = "INSERT INTO STUDENT (ID, FIRSTNAME, LASTNAME, EXPECTED_GRAD_YEAR, MAJOR, EMAIL) VALUES (" +
        to_string(id) + ", '" + fname + "', '" + lname + "', " + to_string(gradYear) + ", '" + major + "', '" + email + "');";

    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK)
        cout << "Student added successfully.\n";
    else
        cerr << "Error adding student: " << sqlite3_errmsg(DB) << endl;
}


void addInstructorToSystem(sqlite3* DB) {
    int id, yearOfHire;
    string fname, lname, title, department, email;

    cout << "Enter instructor ID: ";
    cin >> id;
    cin.ignore(); // clear newline from buffer

    cout << "Enter first name: ";
    getline(cin, fname);

    cout << "Enter last name: ";
    getline(cin, lname);

    cout << "Enter title (e.g., 'Full Prof.', 'Associate Prof.'): ";
    getline(cin, title);

    cout << "Enter year of hire: ";
    cin >> yearOfHire;
    cin.ignore();

    cout << "Enter department: ";
    getline(cin, department);

    cout << "Enter email: ";
    getline(cin, email);

    string sql = "INSERT INTO INSTRUCTOR (ID, FIRSTNAME, SURNAME, TITLE, YEAROFHIRE, DEPARTMENT, EMAIL) VALUES (" +
        to_string(id) + ", '" + fname + "', '" + lname + "', '" + title + "', " + to_string(yearOfHire) +
        ", '" + department + "', '" + email + "');";

    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK)
        cout << "Instructor added successfully.\n";
    else
        cerr << "Error adding instructor: " << sqlite3_errmsg(DB) << endl;
}


void linkInstructorToCourse(sqlite3* DB) {//Dylan
    int crn, instructorID;
    cout << "Enter course CRN: "; cin >> crn;
    cout << "Enter instructor ID: "; cin >> instructorID;

    string sql = "UPDATE COURSE SET INSTRUCTOR = " + to_string(instructorID) + " WHERE CRN = " + to_string(crn) + ";";
    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK)
        cout << "Instructor linked to course.\n";
    else
        cerr << "Error: " << sqlite3_errmsg(DB) << endl;
}

void unlinkInstructorFromCourse(sqlite3* DB) { // Dylan
    int crn;
    cout << "Enter course CRN: ";
    cin >> crn;

    string sql = "UPDATE COURSE SET INSTRUCTOR = NULL WHERE CRN = " + to_string(crn) + ";";

    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK)
        cout << "Instructor unlinked from course.\n";
    else
        cerr << "Error: " << sqlite3_errmsg(DB) << endl;
}


void addStudentToCourse(sqlite3* DB) {//Isabella
    int studentID, crn;
    cout << "Enter student ID: "; cin >> studentID;
    cout << "Enter course CRN: "; cin >> crn;

    string sql = "INSERT INTO ENROLLMENT (STUDENTID, CRN) VALUES (" + to_string(studentID) + ", " + to_string(crn) + ");";
    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK)
        cout << "Student added to course.\n";
    else
        cerr << "Error: " << sqlite3_errmsg(DB) << endl;
}

void removeStudentFromCourse(sqlite3* DB) {//Ergisa
    int studentID, crn;
    cout << "Enter student ID: "; cin >> studentID;
    cout << "Enter course CRN: "; cin >> crn;

    string sql = "DELETE FROM ENROLLMENT WHERE STUDENTID = " + to_string(studentID) + " AND CRN = " + to_string(crn) + ";";
    int rc = sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK)
        cout << "Student removed from course.\n";
    else
        cerr << "Error: " << sqlite3_errmsg(DB) << endl;
}

void printSchedule(sqlite3* DB, int studentID) {//Isabella
    string sql = "SELECT STARTTIME, ENDTIME, DAY FROM SSCHEDULE WHERE ID = " + to_string(studentID) + ";";
    auto cb = [](void*, int argc, char** argv, char** col) -> int {
        for (int i = 0; i < argc; i++) {
            cout << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "------------------\n";
        return 0;
        };
    sqlite3_exec(DB, sql.c_str(), cb, nullptr, nullptr);
}
void printAllCourses(sqlite3* DB) {//Ergisa
    string sql = "SELECT * FROM COURSE;";
    auto cb = [](void*, int argc, char** argv, char** col) -> int {
        for (int i = 0; i < argc; i++) {
            cout << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "------------------\n";
        return 0;
        };
    sqlite3_exec(DB, sql.c_str(), cb, nullptr, nullptr);
}

void searchCourseRoster(sqlite3* DB, int instructorID) {//Dylan
    int crn;
    cout << "Enter CRN: ";
    cin >> crn;

    string sql = "SELECT ID FROM SSCHEDULE WHERE CRN = " + to_string(crn) + ";";
    auto cb = [](void*, int argc, char** argv, char** col) -> int {
        for (int i = 0; i < argc; i++) {
            cout << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        return 0;
        };
    sqlite3_exec(DB, sql.c_str(), cb, nullptr, nullptr);
}

//void printRoster(sqlite3* DB) {
//    string sql = "SELECT * FROM SSCHEDULE;";
//    auto cb = [](void*, int argc, char** argv, char** col) -> int {
//        for (int i = 0; i < argc; i++) {
//            cout << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
//        }
//        cout << "------------------\n";
//        return 0;
//        };
//    sqlite3_exec(DB, sql.c_str(), cb, nullptr, nullptr);
//}

void executeSQL(sqlite3* DB, const string& sql, const char* successMsg = nullptr) {//Ergisa
    char* messageError;
    if (sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError) != SQLITE_OK) {
        cerr << "SQL error: " << messageError << endl;
        sqlite3_free(messageError);
    }
    else if (successMsg) {
        cout << successMsg << endl;
    }
}
void createStudentTable(sqlite3* DB) {//Dylan
    string sql = "CREATE TABLE IF NOT EXISTS STUDENT("
        "ID INTEGER PRIMARY KEY, "
        "FIRSTNAME TEXT NOT NULL, "
        "LASTNAME TEXT NOT NULL, "
        "EXPECTED_GRAD_YEAR INTEGER, "
        "MAJOR TEXT NOT NULL, "
        "EMAIL TEXT);";
    executeSQL(DB, sql, "STUDENT table created (if not existed).");
}
void createInstructorTable(sqlite3* DB) {//Isabella
    string sql = "CREATE TABLE INSTRUCTOR("
        "ID INTEGER PRIMARY KEY, "
        "FIRSTNAME TEXT NOT NULL, "
        "SURNAME TEXT NOT NULL, "
        "TITLE TEXT NOT NULL, "
        "YEAROFHIRE INTEGER, "
        "DEPARTMENT TEXT NOT NULL, "
        "EMAIL TEXT); ";
    executeSQL(DB, sql, "INSTRUCTOR table created (if not existed).");
}
void recreateInstructorTable(sqlite3* DB) {//Ergisa
    executeSQL(DB, "DROP TABLE IF EXISTS INSTRUCTOR;", "Dropped old INSTRUCTOR table.");
    createInstructorTable(DB);
}
void createAdministratorTable(sqlite3* DB) {//Ergisa
    string sql = "CREATE TABLE IF NOT EXISTS ADMINISTRATOR("
        "ID INTEGER PRIMARY KEY, "
        "FIRSTNAME TEXT NOT NULL, "
        "SURNAME TEXT NOT NULL, "
        "TITLE TEXT NOT NULL, "
        "OFFICE TEXT NOT NULL, "
        "EMAIL TEXT); ";
    executeSQL(DB, sql, "ADMINISTRATOR table created (if not existed).");
}
void recreateAdministratorTable(sqlite3* DB) {//Dylan
    executeSQL(DB, "DROP TABLE IF EXISTS ADMINISTRATOR;", "Dropped old ADMINISTRATOR table.");
    createAdministratorTable(DB);
}
void createCourseTable(sqlite3* DB) {//Isabella
    const char* createCourseTableSQL = R"(
    CREATE TABLE IF NOT EXISTS COURSE (
        CRN INTEGER PRIMARY KEY,
        TITLE TEXT,
        ID TEXT,
        DEPARTMENT TEXT,
        STARTTIME INTEGER,
        ENDTIME INTEGER,
        DAY1 TEXT,
        DAY2 TEXT,
        SEMESTER TEXT,
        YEAR INTEGER,
        CREDITS INTEGER,
        INSTRUCTOR INTEGER
    );
)";

    int rc = sqlite3_exec(DB, createCourseTableSQL, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error creating COURSE table: " << sqlite3_errmsg(DB) << endl;
    }
}

void recreateCourseTable(sqlite3* DB) {//Dylan
    executeSQL(DB, "DROP TABLE IF EXISTS COURSE;", "Dropped old COURSE table.");
    createCourseTable(DB);
}
void createLoginTable(sqlite3* DB) {//Isabella
    string sql = "CREATE TABLE IF NOT EXISTS LOGIN("
        "USERNAME TEXT PRIMARY KEY, "
        "PASSWORD TEXT NOT NULL, "
        "ROLES TEXT NOT NULL);";
    executeSQL(DB, sql, "Login table created(if not existed).");
}
void createSScheduleTable(sqlite3* DB) {//Ergisa
    const char* createSScheduleTableSQL = R"(
        CREATE TABLE IF NOT EXISTS SSCHEDULE (
            ID INTEGER,
            STARTTIME INTEGER,
            ENDTIME INTEGER,
            DAY1 TEXT,
            DAY2 TEXT,
            SEMESTER TEXT,
            YEAR INTEGER,
            COURSEID TEXT,
            PRIMARY KEY (ID, STARTTIME, ENDTIME, DAY1, DAY2, SEMESTER, YEAR, COURSEID)
        );
    )";

    char* errorMessage = nullptr;
    int rc = sqlite3_exec(DB, createSScheduleTableSQL, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        cerr << "Error creating SSCHEDULE table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
    else {
        cout << "SSCHEDULE table created (if not existed)." << endl;
    }
}


void recreateSScheduleTable(sqlite3* DB) {//Ergisa
    executeSQL(DB, "DROP TABLE IF EXISTS SSCHEDULE;", "Dropped old SSCHEDULE table.");
    createSScheduleTable(DB);
}

void insertInitialData(sqlite3* DB) {
    //Instructor: Isabella
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2001, 'Joseph', 'Fourier', 'Full Prof.', 1820, 'ELECTRICAL ENGINEERING', 'fourierj@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2002, 'Alan', 'Turing', 'Associate Prof.', 1940, 'COMPUTER ENGINEERING', 'turinga@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2003, 'Nelson', 'Patrick', 'Full Prof.', 1994, 'MECHANICAL ENGINEERING', 'patrickn@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2004, 'Galieleo', 'Galilei', 'Full Prof.', 1600, 'APPLIED SCIENCES', 'galileig@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2005, 'Katie', 'Bouman', 'Associate Prof.', 2019, 'COMPUTER ENGINEERING', 'boumank@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2007, 'Marcus', 'Brooks','Full Prof.', 2010, 'ELECTRICAL ENGINEERING', 'brooksm@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2008, 'Ava', 'Rivers', 'Associate Prof.', 2015, 'COMPUTER ENGINEERING', 'riversa@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2009, 'Leo', 'Hayes', 'Full Prof.', 2010, 'COMPUTER ENGINEERING', 'hayesl@wit.ed'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2010, 'Emma', 'Johnson', 'Associate Prof.', 2020, 'APPLIED SCIENCES', 'johnsone@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2011, 'Liam', 'Parker', 'Associate Prof.', 2017, 'ELECTRICAL ENGINEERING', 'parkerl@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2012, 'Elijah', 'Harris', 'Full Prof.', 2016, 'COMPUTER ENGINEERING', 'harrise@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2013, 'Sophia', 'Clark', 'Full Prof.', 2010, 'APPLIED SCIENCES', 'clarks@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2014, 'Isabella', 'Lewis', 'Full Prof.', 2021, 'COMPUTER ENGINEERING', 'lewisi@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2015, 'Jack', 'Cooper', 'Full Prof.', 2018, 'ELECTRICAL ENGINEERING', 'cooperj@wit.edu'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO INSTRUCTOR VALUES(2016, 'Ian', 'Liam', 'Full Prof.', 2019, 'CIVIL ENGINEERING', 'liami@wit.edu'); ");


    executeSQL(DB, "INSERT OR IGNORE INTO ADMINISTRATOR VALUES(3001, 'Vera', 'Rubin', 'Registar', 'Wentworth 101', 'rubinv@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO ADMINISTRATOR VALUES(3002, 'Margreat', 'Hamilton', 'President', 'Dobbs 1600', 'hamiltonm@wit.edu');");

    //Student: Ergisa
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1001, 'Issac', 'Newton', 1668, 'APPLIED SCIENCES', 'newtoni@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1002, 'Marie', 'Curie', 1903, 'APPLIED SCIENCES', 'curiem@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1003, 'Nikola', 'Tesla', 1878, 'ELECTRICAL ENGINEERING', 'telsan@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1004, 'Thomas', 'Edison', 1879, 'ELECTRICAL ENGINEERING', 'notcool@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1005, 'John', 'von Neumann', 1923, 'COMPUTER ENGINEERING', 'vonneumannji@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1006, 'Grace', 'Hopper', 1928, 'COMPUTER SCIENCE', 'hopperg@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1007, 'Mae', 'Jemison', 1981, 'CHEMISTRY', 'jemisonm@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1008, 'Mark', 'Dean', 1979, 'COMPUTER ENGINEERING', 'deanm@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1009, 'Michael', 'Faraday', 1812, 'APPLIED SCIENCES', 'faradaym@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1010, 'Ada', 'Lovelace', 1832, 'COMPUTER SCIENCE', 'lovelacea@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES(1011, 'Layla', 'Brown', 2028, 'COMPUTER ENGINEERING', 'brownl@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1012, 'Rose', 'Snyder', 2027, 'COMPUTER SCIENCE', 'snyderr@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1013, 'Bobbino', 'Bambino', 2030, 'ELECTRICAL ENGINEERING', 'bambinob@wit.edu');");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1014, 'Oli', 'Lela', 2025, 'MECHANICAL ENGINEERING', 'lelao@wit.edu') ;");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1015, 'Powli', 'Noonan', 2026, 'ELECTRICAL ENGINEERING', 'noonanp@wit.edu') ;");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1016, 'Neis', 'Freeman', 2027, 'COMPUTER ENGINEERING', 'freemann@wit.edu') ;");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1017, 'Heit', 'Lan', 2028, 'COMPUTER ENGINEERING', 'lanh@wit.edu') ;");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1018, 'Powey', 'Min', 2030, 'COMPUTER SCIENCE', 'minp@wit.edu') ;");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1019, 'Ilia', 'Unisa', 2028, 'MECHANICAL ENGINEERING', 'unisai@wit.edu') ;");
    executeSQL(DB, "INSERT OR IGNORE INTO STUDENT VALUES (1020, 'Lowry',' Leison', 2030, 'ELECTRICAL ENGINEERING', 'leisonl@wit.edu') ;");
    //Login values: Ergisa/Isabella
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('rubinv@wit.edu', 'rubin11', 'ADMINISTRATOR');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('hamiltonm@wit.edu', 'hamilton11', 'ADMINISTRATOR');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('curiem@wit.edu', 'currie10', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('telsan@wit.edu', 'telsan44', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('snyderr@wit.edu' ,'Snyd324', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('notcool@wit.edu', 'notcool22', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('vonneumannji@wit.edu', 'von32', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('hopperg@wit.edu', 'aihop65', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('jemisonm@wit.edu', 'light7', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('deanm@wit.edu', 'martini21', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('faradaym@wit.edu', 'farais34', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('lovelacea@wit.edu', 'loveyui98', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('newtoni@wit.edu', 'newton11', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('brownl@wit.edu', 'Olaf2023', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('snyderr@wit.edu' ,'Snyd324', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('bambinob@wit.edu', 'bebinen67', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('lelao@wit.edu', 'leis98', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('noonanp@wit.edu', 'dorito32', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('freemann@wit.edu', 'neigh1', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('lanh@wit.edu', 'leys54', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('minp@wit.edu', 'loeis665', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('unisai@wit.edu', 'neeity76', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES ('leisonl@wit.edu', 'power434', 'STUDENT');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('brooksm@wit.edu', 'brooksm26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('riversa@wit.edu', 'riversa26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('hayesl@wit.edu', 'hayesl26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('johnsone@wit.edu', 'johnsone26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('parkerl@wit.edu', 'parkerl26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('harrise@wit.edu', 'harrise26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('clarks@wit.edu', 'clarks26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('lewisi@wit.edu', 'lewisi26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('cooperj@wit.edu', 'cooperj26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('liami@wit.edu', 'liami26', 'INSTRUCTOR'); ");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('fourierj@wit.edu', 'fourier11', 'INSTRUCTOR');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('turinga@wit.edu', 'allier43', 'INSTRUCTOR');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('patrickn@wit.edu', 'patty38', 'INSTRUCTOR');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('galileig@wit.edu', 'galiii098', 'INSTRUCTOR');");
    executeSQL(DB, "INSERT OR IGNORE INTO LOGIN VALUES('boumank@wit.edu', 'bouiiiiww32', 'INSTRUCTOR');");
    //Course values: Ergisa
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(123, 'APPLIED PROGRAMMING CONCEPTS','ELEC 3200', 'COMPUTER ENGINEERING', 10, 12, 'TUESDAY', 'THURSDAY', 'SUMMER', 2025, 3,2002);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(456, 'COMPUTER NETWORKS', 'ELEC 3300' ,'COMPUTER ENGINEERING', 12, 14, 'TUESDAY','THURSDAY', 'SUMMER', 2025, 4,2009);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(789, 'SIGNALS AND SYSTEMS', 'ELEC 3400' ,'ELECTRICAL ENGINEERING', 10, 12, 'MONDAY','WEDNESDAY', 'SUMMER', 2025, 4,2001);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(226, 'SENIOR DESIGN II', 'ELEC 3500','COMPUTER ENGINEERING', 12, 14, 'MONDAY','WEDNESDAY', 'SUMMER', 2025, 4,2014);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(413, 'ADVANCED DIGITAL CIRCUIT', 'ELEC 3600','ELECTRICAL ENGINEERING', 3, 5, 'TUESDAY','THURSDAY', 'SUMMER', 2025, 4,2007);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(121, 'NETWORK THEORY 1','ELEC 2800', 'ELECTRICAL ENGINEERING', 8, 9, 'TUESDAY','THURSDAY', 'SPRING', 2025, 4,2011);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(122, 'NETWORK THEORY 2','ELEC 2900', 'ELECTRICAL ENGINEERING', 8, 9, 'TUESDAY','THURSDAY', 'FALL', 2025, 4,2001);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(124, 'DIGITAL LOGIC','ELEC 3000', 'COMPUTER ENGINEERING', 10, 11, 'WEDNESDAY','FRIDAY', 'SUMMER', 2025, 4,2009);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(133, 'COMPUTER ARCHITECTURE','ELEC 3100', 'COMPUTER ENGINEERING', 10, 12, 'MONDAY','THURSDAY', 'SUMMER', 2025, 4,2005);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(144, 'COMP SCI 1','COMP 3000', 'COMPUTER SCIENCE', 10, 12, 'TUESDAY','FRIDAY', 'SUMMER', 2025, 4,2013);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(155, 'COMP SCI 2','COMP 3100', 'COMPUTER SCIENCE', 11, 13, 'MONDAY','THURSDAY', 'SPRING', 2025, 4,2013);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(166, 'INTRO TO MECHANICS','MECH 2700', 'MECHANICAL ENGINEERING', 12, 14, 'MONDAY','THURSDAY', 'SPRING', 2025, 4,2003);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(177, 'INTRO TO FLUIDS','CIV 2600', 'CIVIL ENGINEERING', 13, 15, 'MONDAY','FRIDAY', 'FALL', 2025, 4,2016);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(188, 'PYTHON LANGUAGE','ELEC 3000', 'COMPUTER ENGINEERING', 11, 14, 'MONDAY','FRIDAY', 'SPRING', 2025, 4,2005);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(199, 'OBJECT ORIENTED PROGRAMMING','ELEC 3900', 'COMPUTER ENGINEERING', 12, 14, 'MONDAY','WEDNESDAY', 'SPRING', 2025, 4,2009);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(201, 'INTRO TO C++','COMP 2400', 'COMPUTER SCIENCE', 11, 12, 'TUESDAY','THURSDAY', 'FALL', 2025, 3,2013);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(210, 'FLUIDS AND MECHANICS','MECH 3500', 'MECHANICAL ENGINEERING', 12, 14, 'MONDAY','FRIDAY', 'SUMMER', 2025, 4,2003);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(340, 'SOIL MECHANICS','CIV 2300', 'CIVIL ENGINEERING', 12, 14, 'MONDAY','THURSDAY', 'SPRING', 2025, 4,2016);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(329, 'SQL LEARNING','COMP 2200', 'COMPUTER SCIENCE', 10, 11, 'MONDAY','----', 'SPRING', 2025, 2,2013);");
    executeSQL(DB, "INSERT OR IGNORE INTO COURSE VALUES(470, 'COMPUTER HARDWARE','ELEC 2900', 'COMPUTER ENGINEERING', 14, 16, 'MONDAY','TUESDAY', 'FALL', 2025, 4,2008);");

    executeSQL(DB, "INSERT OR IGNORE INTO SSCHEDULE VALUES(1999, 8 , 9, 'TUESDAY', 'THURSDAY', 'SPRING', 2025, 'ELEC 2800');");
}
void updateAdminTitle(sqlite3* DB) {//Ergisa
    string sql = "UPDATE ADMINISTRATOR SET TITLE = 'Vice-President' WHERE SURNAME = 'Rubin';";
    executeSQL(DB, sql, "Vera Rubin's title updated to Vice-President.");
}
void printAllFromTable(sqlite3* DB, const string& table) {//Dylan
    string sql = "SELECT * FROM " + table + ";";
    cout << "\nAll entries from " << table << ":\n";
    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
}
void insertStudent(sqlite3* DB) {//Isabella
    int id, year;
    string firstName, lastName, major, email;
    cout << "Insert Student\nEnter ID: "; cin >> id;
    cout << "First name: "; cin >> firstName;
    cout << "Last name: "; cin >> lastName;
    cout << "Expected graduation year: "; cin >> year;
    cout << "Major: "; cin.ignore(); getline(cin, major);
    cout << "Email: "; getline(cin, email);

    string sql = "INSERT INTO STUDENT (ID, FIRSTNAME, LASTNAME, EXPECTED_GRAD_YEAR, MAJOR, EMAIL) VALUES("
        + to_string(id) + ", '" + firstName + "', '" + lastName + "', " + to_string(year) + ", '" + major + "', '" + email + "');";
    executeSQL(DB, sql, "Student inserted.");
}

void insertInstructor(sqlite3* DB) {//Ergisa
    int id, YEAROFHIRE;
    string firstName, surName, title, department, email;
    cout << "Insert Instructor\nEnter ID: "; cin >> id;
    cout << "First name: "; cin >> firstName;
    cout << "Surname: "; cin >> surName;
    cout << "Title: "; cin >> title;
    cout << "Year of Hire: "; cin >> YEAROFHIRE;
    cout << "Department: "; cin.ignore(); getline(cin, department);
    cout << "Email: "; getline(cin, email);

    string sql = "INSERT INTO INSTRUCTOR (ID, FIRSTNAME, SURNAME, TITLE, YEAROFHIRE, DEPARTMENT, EMAIL) VALUES("
        + to_string(id) + ", '" + firstName + "', '" + surName + "', '" + title + "', "
        + to_string(YEAROFHIRE) + ", '" + department + "', '" + email + "');";
    executeSQL(DB, sql, "Instructor inserted.");

}

void insertLogin(sqlite3* DB) {//Dylan
    string username, password, role;
    cout << "Setp up Login\nEnter Email: "; cin >> username;
    cout << "Password: "; cin >> password;
    cout << "Role: "; cin.ignore(); getline(cin, role);

    string sql = "INSERT OR IGNORE INTO LOGIN (USERNAME, PASSWORD, ROLES) VALUES('"
        + username + "', '" + password + "', '" + role + "');";
    executeSQL(DB, sql, "Login Inserted.");
}

void addCourseToSystem(sqlite3* DB) { // Ergisa
    int crn, year, credits;
    string title, department, semester, ID;
    string day1, day2;
    int start, end;

    cout << "Insert Course\nEnter CRN: "; cin >> crn;
    cout << "Title: "; cin.ignore(); getline(cin, title);
    cout << "ID: "; getline(cin, ID);
    cout << "Department: "; getline(cin, department);
    cout << "Day 1 (e.g., Mon): "; getline(cin, day1);
    cout << "Day 2 (leave blank if none): "; getline(cin, day2);
    cout << "Semester: "; getline(cin, semester);
    cout << "Year: "; cin >> year;
    cout << "Credits: "; cin >> credits;
    cout << "Start Time (24hr format): "; cin >> start;
    cout << "End Time (24hr format): "; cin >> end;

    string sql = "INSERT INTO COURSE (CRN, TITLE, ID, DEPARTMENT, STARTTIME, ENDTIME, DAY1, DAY2, SEMESTER, YEAR, CREDITS) VALUES(" +
        to_string(crn) + ", '" + title + "', '" + ID + "', '" + department + "', " +
        to_string(start) + ", " + to_string(end) + ", '" + day1 + "', '" + day2 + "', '" +
        semester + "', " + to_string(year) + ", " + to_string(credits) + ");";

    executeSQL(DB, sql, "Course inserted.");
}


void removeStudent(sqlite3* DB) {//Dylan
    int id;
    cout << "Enter Student ID to remove: ";
    cin >> id;

    string sql = "DELETE FROM STUDENT WHERE ID = " + to_string(id) + ";";
    executeSQL(DB, sql, "Student removed if existed.");
}

void removeInstructor(sqlite3* DB) {//Isabella
    int id;
    cout << "Enter Instructor ID to remove: ";
    cin >> id;

    string sql = "DELETE FROM INSTRUCTOR WHERE ID = " + to_string(id) + ";";
    executeSQL(DB, sql, "Instructor removed if existed.");
}

void removeCourse(sqlite3* DB) {//Dylan
    int crn;
    cout << "Enter Course CRN to remove: ";
    cin >> crn;

    string sql = "DELETE FROM COURSE WHERE CRN = " + to_string(crn) + ";";
    executeSQL(DB, sql, "Course removed if it existed.");
}
void printStudentSchedule(sqlite3* DB, int studentID) {//dylan
    cout << "\nYour Schedule:\n";

    string sql = R"(
        SELECT C.ID, C.TITLE, C.STARTTIME, C.ENDTIME, C.DAY1, C.DAY2, C.SEMESTER, C.YEAR
        FROM COURSE C
        JOIN SSCHEDULE S
        ON C.ID = S.COURSEID
           AND C.STARTTIME = S.STARTTIME
           AND C.ENDTIME = S.ENDTIME
           AND C.SEMESTER = S.SEMESTER
           AND C.YEAR = S.YEAR
        WHERE S.ID = )" + to_string(studentID) + R"(;
    )";

    auto callback = [](void*, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i < argc; i++) {
            cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "------------------\n";
        return 0;
        };

    int rc = sqlite3_exec(DB, sql.c_str(), callback, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to retrieve schedule: " << sqlite3_errmsg(DB) << endl;
    }
}


void updateStudentEmail(sqlite3* DB) {//Ergisa
    int id;
    string newEmail;
    cout << "Enter Student ID to update email: ";
    cin >> id;
    cout << "Enter new email: ";
    cin.ignore();
    getline(cin, newEmail);

    string sql = "UPDATE STUDENT SET EMAIL = '" + newEmail + "' WHERE ID = " + to_string(id) + ";";
    executeSQL(DB, sql, "Student email updated.");
}

void recreateLoginTable(sqlite3* DB) {//Ergisa
    executeSQL(DB, "DROP TABLE IF EXISTS LOGIN;", "Dropped old LOGIN table.");
    createLoginTable(DB);
}

void searchStudentByID(sqlite3* DB) {//Dylan
    int id;
    cout << "Enter Student ID to search: ";
    cin >> id;

    string sql = "SELECT * FROM STUDENT WHERE ID = " + to_string(id) + ";";
    cout << "Search results:\n";
    sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
}

int lcallback(void* data, int argc, char** argv, char** azColName) {//Isabella
    if (argv[0]) {
        *((string*)data) = argv[0];
    }
    return 0;
}

string currentUserID = ""; // Global or passed reference variable to store ID

string checklogin(sqlite3* DB, string& currentUserID) {//Isabella
    while (true) {
        string role;
        string username;
        string password;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin.ignore(); getline(cin, password);

        string sql = "SELECT ROLES FROM LOGIN WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "';";
        role.clear();
        sqlite3_exec(DB, sql.c_str(), lcallback, &role, nullptr);

        if (!role.empty()) {
            cout << "Login Successful.\n";

            // If it's a student, retrieve the student ID
            if (role == "STUDENT") {
                string findID = "SELECT ID FROM STUDENT WHERE EMAIL = '" + username + "';";
                sqlite3_exec(DB, findID.c_str(), lcallback, &currentUserID, nullptr);
            }

            return role;
        }
        else {
            cout << "Login failed. Invalid Username or Password. Try Again.\n\n";
        }
    }
}

void addCourseToSchedule(sqlite3* DB, int studentID) {//Ergisa
    string courseID;
    cout << "Enter Course ID (e.g., ELEC 3000): ";
    cin >> ws;  
    getline(cin, courseID);

   
    struct Course {
        int crn;
        string title;
        int start;
        int end;
        string day1;
        string day2;
        string semester;
        int year;
    };

    vector<Course> matchingCourses;

    string sql = "SELECT CRN, TITLE, STARTTIME, ENDTIME, DAY1, DAY2, SEMESTER, YEAR FROM COURSE WHERE ID = '" + courseID + "';";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        vector<Course>* courses = static_cast<vector<Course>*>(data);
        if (argc == 8 && argv[0] && argv[1] && argv[2] && argv[3] && argv[4] && argv[5] && argv[6] && argv[7]) {
            Course c;
            c.crn = atoi(argv[0]);
            c.title = argv[1];
            c.start = atoi(argv[2]);
            c.end = atoi(argv[3]);
            c.day1 = argv[4];
            c.day2 = argv[5];
            c.semester = argv[6];
            c.year = atoi(argv[7]);
            courses->push_back(c);
        }
        return 0;
    };



    int rc = sqlite3_exec(DB, sql.c_str(), callback, &matchingCourses, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error retrieving courses: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    if (matchingCourses.empty()) {
        cout << "No courses found with ID: " << courseID << endl;
        return;
    }

    cout << "\nMatching Courses:\n";
    for (const auto& c : matchingCourses) {
        cout << "CRN: " << c.crn << ", Title: " << c.title
            << ", Time: " << c.start << "-" << c.end
            << ", Days: " << c.day1 << "/" << c.day2
            << ", Semester: " << c.semester << ", Year: " << c.year
             << endl;
    }

  
    int selectedCRN;
    cout << "\nEnter the CRN of the course you want to add: ";
    cin >> selectedCRN;

    auto it = std::find_if(matchingCourses.begin(), matchingCourses.end(),
        [selectedCRN](const Course& c) { return c.crn == selectedCRN; });

    if (it == matchingCourses.end()) {
        cout << "Invalid CRN selected.\n";
        return;
    }

    Course selected = *it;

    bool conflict = false;
    string conflictSQL = "SELECT 1 FROM SSCHEDULE WHERE ID = " + to_string(studentID) +
        " AND SEMESTER = '" + selected.semester + "'" +
        " AND YEAR = " + to_string(selected.year) +
        " AND (" +
        "(" + to_string(selected.start) + " < ENDTIME AND " + to_string(selected.end) + " > STARTTIME)" +  // time overlap
        " AND (" +
        "DAY1 = '" + selected.day1 + "' OR DAY1 = '" + selected.day2 + "' OR " +
        "DAY2 = '" + selected.day1 + "' OR DAY2 = '" + selected.day2 + "'" +
        ")" +
        ");";



    auto conflictCallback = [](void* data, int, char**, char**) -> int {
        *(bool*)data = true;
        return 0;
        };

    sqlite3_exec(DB, conflictSQL.c_str(), conflictCallback, &conflict, nullptr);

    if (conflict) {
        cout << "Schedule conflict detected with another course.\n";
        return;
    }


    string confirm;
    cout << "No conflict. Add this course to your schedule? (yes/no): ";
    cin >> ws;
    getline(cin, confirm);

    if (confirm != "yes") {
        cout << "Course not added.\n";
        return;
    }

    string insertSQL = "INSERT INTO SSCHEDULE (ID, STARTTIME, ENDTIME, DAY1, DAY2, SEMESTER, YEAR, COURSEID) VALUES (" +
        to_string(studentID) + ", " +
        to_string(selected.start) + ", " +
        to_string(selected.end) + ", '" +
        selected.day1 + "', '" +
        selected.day2 + "', '" +
        selected.semester + "', " +
        to_string(selected.year) + ", '" +
        courseID + "');";


    rc = sqlite3_exec(DB, insertSQL.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error inserting course into schedule: " << sqlite3_errmsg(DB) << endl;
    }
    else {
        cout << "Course added to schedule successfully.\n";
    }
}


int main(int argc, char** argv) {
    sqlite3* DB;
    int exit = sqlite3_open("assignment3.db", &DB);
    if (exit != SQLITE_OK) {
        cerr << "Error opening database\n";
        return -1;
    }


    createStudentTable(DB);
    recreateInstructorTable(DB);
    recreateAdministratorTable(DB);
    createCourseTable(DB);
    createLoginTable(DB);
    createSScheduleTable(DB);
    insertInitialData(DB);
    
    string currentUserID = ""; //Dylan
    string roless = checklogin(DB, currentUserID);
    string student = "STUDENT";
    if (roless == student) {
        int choice;
        int studentID = stoi(currentUserID);
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
                
                case 1: searchCoursesByID(DB); break;//switch cases: Ryan
                case 2: searchCourseByCRN(DB); break;
                case 3: addCourseToSchedule(DB, studentID); break;
                case 4: removeCourseFromSchedule(DB, studentID); break;
                case 5: printStudentSchedule(DB, studentID); break;
                case 6: cout << "Exiting" << endl; break;
                default: cout << "Invalid choice" << endl; break;
                

         


            }
            
        } while (choice != 6);
    }
    else if (roless == "INSTRUCTOR") {//Isabella
        int choice;
        int instructorID;
        cout << "Enter your Instructor ID: ";
        cin >> instructorID;

        do {
            cout << "\nMenu:\n"
                << "1. Search Courses by ID\n"
                << "2. Search courses by CRN\n"
                << "3. Print schedule\n"
                << "4. search course roster\n"
                << "5. Print roster\n"
                << "6. Exit\n"
                << "Enter choice\n";
            cin >> choice;
            switch (choice) {
               
                case 1: searchCoursesByID(DB); break;
                case 2: searchCourseByCRN(DB); break;
                case 3: printInstructorSchedule(DB, instructorID); break;
                case 4: searchCourseRoster(DB, instructorID); break;
                case 5: printRoster(DB, instructorID); break;
                case 6: cout << "Exiting" << endl; break;
                default: cout << "Invalid choice" << endl; break;
                


            }

        } while (choice != 6);

    }
    else if (roless == "ADMINISTRATOR") {//Ergisa
        int choice;
        do {
            cout << "\nMenu:\n"
                << "1. Search Courses by ID\n"
                << "2. Search courses by CRN\n"
                << "3. Add courses to system\n"
                << "4. Add student to system\n"
                << "5. Add instructor to system\n"
                << "6. Link Instructor to course\n"
                <<"7. Unlink instructor to course\n"
                <<"8. Add a student to a course\n "
                <<"9. Remove student from course\n"
                <<"10. Exit\n"
                << "Enter choice\n";
            cin >> choice;
            switch (choice) {
            case 1: searchCourseByID(DB); break;
            case 2: searchCourseByCRN(DB); break;
            case 3: addCourseToSystem(DB); break;
            case 4: addStudentToSystem(DB); break;
            case 5: addInstructorToSystem(DB); break;
            case 6: linkInstructorToCourse(DB); break;
            case 7: unlinkInstructorFromCourse(DB); break;
            case 8: addStudentToCourse(DB); break;
            case 9: removeStudentFromCourse(DB); break;
            case 10: cout << "Exiting" << endl; break;
            default: cout << "Invalid choice" << endl; break;
            }


            

        } while (choice != 10);

    }

    sqlite3_close(DB);
    return 0;
}
