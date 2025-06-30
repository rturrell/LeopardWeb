#include "User.h"
#include <iostream>

using std::cout;
using std::endl;


//Constructor
User::User() {
    firstname = "Dave";
    lastname = "Fernandez";
    id = 282613;
}
User::User(string in_first, string in_last, int in_id) {
    firstname = in_first;
    lastname = in_last;
    id = in_id;
}

//Methods
string User::show_first() {
    return "First name: " + firstname;
}

string User::show_last() {
    return lastname;
}

int User::show_id() {
    return id;
}

void User::show_all() {
    cout << "First name: " << firstname << endl << "Last name: " << lastname << endl << "ID: " << id << endl;
}

//Destructor
User::~User() {

}
