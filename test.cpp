#include "pch.h"
#include <sqlite3.h>
#include "C:\Users\decubellisi\Documents\Applied Programming\ConsoleApplication_Testing_WIT1\myfunc.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

TEST(LoginTest, ValidStudentLogin) { //Dylan
    sqlite3* DB;
    sqlite3_open("assignment3.db", &DB);

    string currentUserID = "1001"; 
    string password = "newton11";
    string username = "newtoni@wit.edu";

    EXPECT_EQ("STUDENT", checklogin(DB, username, password, currentUserID));
    EXPECT_FALSE(currentUserID.empty());

    sqlite3_close(DB);
}

TEST(LoginTest, ValidInstructorLogin2) { //Ergisa 
    sqlite3* DB;
    sqlite3_open("assignment3.db", &DB);

    string currentUserID = "2001";
    string password = "fourier11";
    string username = "fourierj@wit.edu";

    EXPECT_EQ("INSTRUCTOR", checklogin(DB, username, password, currentUserID));
    EXPECT_FALSE(currentUserID.empty());

    sqlite3_close(DB);
}

TEST(LoginTest, ValidAdminLogin3) {//Isabella 
    sqlite3* DB;
    sqlite3_open("assignment3.db", &DB);

    string currentUserID = "3001";
    string password = "rubin11";
    string username = "rubinv@wit.edu";

    EXPECT_EQ("ADMINISTRATOR", checklogin(DB, username, password, currentUserID));
    EXPECT_FALSE(currentUserID.empty());

    sqlite3_close(DB);
}
//WE KEPT THE FOLLOWING CODE COMMENTED OUT WHEN TESTING OTHERWISE IT WOULD COUNTERACT EACH OTHER FOR CERTAIN TEST CASES.
// HOWEVER THIS CODE WAS USED.
/*
TEST(AddCourseToScheduleTest, ValidAdd) { //Ergisa 
    sqlite3* DB;
    ASSERT_EQ(SQLITE_OK, sqlite3_open("assignment3.db", &DB));

    int studentID = 1001;
    string courseID = "ELEC 2800";
    int crn = 121;              // Must match a CRN in your DB
    bool confirm = true;

    EXPECT_TRUE(addCourseToSchedule(DB, studentID, courseID, crn, confirm));

    sqlite3_close(DB);
}
*/
/*
TEST(RemoveCourseTest, ValidRemoval) { //Dylan
    sqlite3* DB;
    ASSERT_EQ(sqlite3_open("assignment3.db", &DB), SQLITE_OK);

    int studentID = 120;
    std::string courseID = "ELEC 3000";

     You might want to ensure the course is present before removal in setup
    bool removed = removeCourseFromSchedule(DB, studentID, courseID);
    EXPECT_TRUE(removed);

    sqlite3_close(DB);
}
*/
/*
TEST(PrintRosterTest, ValidInstructorAndCRN) { //Ryan
    sqlite3* DB;
    ASSERT_EQ(sqlite3_open("assignment3.db", &DB), SQLITE_OK);

    int instructorID = 2001;
    int crn = 121;  // CRN the instructor is assigned to

    string courseTitle;
    vector<tuple<int, string, string>> roster;

    bool success = getCourseRoster(DB, instructorID, crn, courseTitle, roster);
    EXPECT_TRUE(success);
    EXPECT_FALSE(courseTitle.empty());
    EXPECT_FALSE(roster.empty());

    for (const auto& student : roster) {
        cout << "Student ID: " << std::get<0>(student)
            << ", First: " << std::get<1>(student)
            << ", Last: " << std::get<2>(student) << "\n";
    }

    sqlite3_close(DB);
}
*/
/*
TEST(AddCourseToSystemTest, ValidInsert) { //Ryan
    sqlite3* DB;
    ASSERT_EQ(SQLITE_OK, sqlite3_open("assignment3.db", &DB));

    bool success = addCourseToSystem(
        DB,
        999,                      // CRN
        "STATS",    // Title
        "MATH 3500",                // ID
        "MATHMATICS",                     // Department
        "Mon",                      // Day 1
        "Wed",                      // Day 2
        "Fall",                     // Semester
        2025,                       // Year
        3,                          // Credits
        10,                       // Start Time
        11                        // End Time
    );

    EXPECT_TRUE(success);

    sqlite3_close(DB);
}
*/
/*
TEST(RemoveCourseTest, ValidDeleteConfirmed) { //Ergisa 
    sqlite3* DB;
    ASSERT_EQ(SQLITE_OK, sqlite3_open("assignment3.db", &DB));

    int crn = 999;  // make sure this CRN exists first
    string removedTitle;

    bool result = removeCourseFromSystem(DB, crn, true, &removedTitle);
    EXPECT_TRUE(result);
    EXPECT_FALSE(removedTitle.empty());

    sqlite3_close(DB);
}
*/

/*
TEST(SearchCoursesByIDTest, FindsExistingCourse) { //Dylan
    sqlite3* DB;
    ASSERT_EQ(sqlite3_open("assignment3.db", &DB), SQLITE_OK);

    vector<map<string, string>> results;
    bool success = searchCoursesByID(DB, "ELEC 3000", results);

    EXPECT_TRUE(success);

    if (!results.empty()) {
        cout << "Courses with ID 'ELEC 3000':\n";
        for (const auto& row : results) {
            for (const auto& pair : row) {
                cout << pair.first << ": " << pair.second << "\n";
            }
            cout << "------------------\n";
        }
    }
    else {
        cout << "No courses found with ID 'ELEC 3000'\n";
    }

    sqlite3_close(DB);
}
*/
TEST(SearchCourseByCRNTest, CourseMayExistInDB) { //Ryan
    sqlite3* DB;
    ASSERT_EQ(sqlite3_open("assignment3.db", &DB), SQLITE_OK);

    vector<map<string, string>> results;

    // Use a CRN that already exists in your database
    int testCRN = 852; // Replace with a CRN that actually exists in your DB

    bool success = searchCourseByCRN(DB, testCRN, results);
    EXPECT_TRUE(success); // sqlite3_exec should not fail

    if (!results.empty()) {
        cout << "Course found:\n";
        for (const auto& row : results) {
            for (const auto& col : row) {
                cout << col.first << ": " << col.second << "\n";
            }
            cout << "------------------\n";
        }
    }
    else {
        cout << "No course found with CRN " << testCRN << "\n";
    }

    sqlite3_close(DB);
}
