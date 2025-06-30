#include "STUDENT.h"
#include<iostream>
using std::cout;
using std::endl;

//Constructor
STUDENT::STUDENT() : User() {

}

STUDENT::STUDENT(string in_first, string in_last, int in_id) : User(in_first, in_last, in_id) {

}

//Methods
void STUDENT::search() {
    cout << "These are the available classes: " << endl;


}

void STUDENT::add_drop() {
    cout << "Class add/drop: " << endl;

}

void STUDENT::prints() {
    cout << "Schedule is: " << endl;

}

//Destructor
STUDENT::~STUDENT() {

}
