#pragma once
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <tuple>
#include <vector>
#include <map>


using std::vector;

using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName);

void printCoursesWithInstructors(sqlite3* DB);

bool searchCoursesByID(sqlite3* DB, const string& id, vector<map<string, string>>& results);

bool getCourseRoster(
    sqlite3* DB,
    int instructorID,
    int crn,
    string& courseTitle,
    vector<tuple<int, string, string>>& roster // ID, First, Last
);

void printInstructorSchedule(sqlite3* DB, int instructorID);

bool searchCourseByCRN(sqlite3* DB, int crn, vector<map<string, string>>& results);

bool removeCourseFromSchedule(sqlite3* DB, int studentID, const string& courseID);

void searchCourseByID(sqlite3* DB);

void searchCourseByDay(sqlite3* DB);

void addStudentToSystem(sqlite3* DB);

void addInstructorToSystem(sqlite3* DB);

void linkInstructorToCourse(sqlite3* DB);

void unlinkInstructorFromCourse(sqlite3* DB);

void addStudentToCourse(sqlite3* DB);

void removeStudentFromCourse(sqlite3* DB);

void printSchedule(sqlite3* DB, int studentID);

void printAllCourses(sqlite3* DB);

void searchCourseRoster(sqlite3* DB, int instructorID);

void executeSQL(sqlite3* DB, const string& sql, const char* successMsg = nullptr);

void createStudentTable(sqlite3* DB);

void createInstructorTable(sqlite3* DB);

void recreateInstructorTable(sqlite3* DB);

void createAdministratorTable(sqlite3* DB);

void recreateAdministratorTable(sqlite3* DB);

void createCourseTable(sqlite3* DB);

void recreateCourseTable(sqlite3* DB);

void createLoginTable(sqlite3* DB);

void createSScheduleTable(sqlite3* DB);

void recreateSScheduleTable(sqlite3* DB);

void createCourseRosterTable(sqlite3* DB);

void recreateCourseRosterTable(sqlite3* DB);

void insertInitialData(sqlite3* DB);

void updateAdminTitle(sqlite3* DB);

void printAllFromTable(sqlite3* DB, const string& table);

void insertStudent(sqlite3* DB);

void insertInstructor(sqlite3* DB);

void insertLogin(sqlite3* DB);

bool addCourseToSystem(
    sqlite3* DB,
    int crn,
    const string& title,
    const string& ID,
    const string& department,
    const string& day1,
    const string& day2,
    const string& semester,
    int year,
    int credits,
    int start,
    int end
);

bool removeCourseFromSystem(
    sqlite3* DB,
    int crn,
    bool confirmDelete,
    string* removedCourseTitle = nullptr
);

void removeStudent(sqlite3* DB);

void removeInstructor(sqlite3* DB);

void removeCourse(sqlite3* DB);

void updateStudentEmail(sqlite3* DB);

void recreateLoginTable(sqlite3* DB);

void searchStudentByID(sqlite3* DB);

int lcallback(void* data, int argc, char** argv, char** azColName);

string checklogin(sqlite3* DB, const string& username, const string& password, string& currentUserID);

bool addCourseToSchedule(
    sqlite3* DB,
    int studentID,
    const string& courseID,
    int selectedCRN,
    bool confirmAdd
);


void printStudentSchedule(sqlite3* DB, int studentID);

