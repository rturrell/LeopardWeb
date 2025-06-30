#include "INSTRUCTOR.h"
#include <iostream>
using std::cout;
using std::endl;

//Constructors
INSTRUCTOR::INSTRUCTOR() : User() {


}
INSTRUCTOR::INSTRUCTOR(string in_first, string in_last, int in_id) : User(in_first, in_last, in_id) {

}

//Methods
void INSTRUCTOR::show_classlist() {
    cout << "Class list is: " << endl;
}

void INSTRUCTOR::show_schedule() {
    cout << "Instructor schedule is: " << endl;


}

void INSTRUCTOR::show_search() {
    cout << "Searching: " << endl;

}

//Destructor
INSTRUCTOR::~INSTRUCTOR() {

}


