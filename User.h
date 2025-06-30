#pragma once

#include <string>
using std::string;


class User
{
protected:
	//Attributes
	string firstname;
	string lastname;
	int id;

public:

	//Constructor
	User();
	User(string in_first, string in_last, int in_id);

	//Methods
	virtual string show_first();
	virtual string show_last();
	virtual int show_id();
	virtual void show_all();

	//Destructor
	~User();
};


