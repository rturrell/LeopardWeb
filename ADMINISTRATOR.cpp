#include "ADMINISTRATOR.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

ADMINISTRATOR::ADMINISTRATOR() :User() {

}
ADMINISTRATOR::ADMINISTRATOR(string in_first, string in_last, int in_id) :User(in_first, in_last, in_id) {

}


//Methods
void ADMINISTRATOR::add_remove_course() {
    int in1;
    cout << "You are adding/ removing course: " << endl;
    cout << "Would you like to add (1) or remove a course(2)? " << endl;
    cin >> in1;
    if (in1 == 1) {
        cout << "Adding course" << endl;

    }
    else {
        cout << "Removing a course" << endl;

    }

}
void ADMINISTRATOR::add_remove_user() {
    int in1;
    cout << "You are adding/removing user: " << endl;
    cout << "Would you like to add (1) or remove a user(2)? " << endl;
    cin >> in1;
    if (in1 == 1) {
        cout << "Adding user" << endl;

    }
    else {
        cout << "Removing a user" << endl;

    }

}
void ADMINISTRATOR::add_remove_student() {
    int in1;
    cout << "You are adding/removing student:" << endl;
    cout << "Would you like to add (1) or remove a student(2)? " << endl;
    cin >> in1;
    if (in1 == 1) {
        cout << "Adding student" << endl;

    }
    else {
        cout << "Removing a student" << endl;

    }

}

void ADMINISTRATOR::search_print_courses() {
    int in1;
    cout << "You are searching/ printing courses: " << endl;
    cout << "Would you like to search (1) or print courses (2)? " << endl;
    cin >> in1;
    if (in1 == 1) {
        cout << "Search courses" << endl;

    }
    else {
        cout << "Print courses" << endl;

    }
}


//Destructor
ADMINISTRATOR::~ADMINISTRATOR() {

}
