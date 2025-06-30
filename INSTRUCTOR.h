#pragma once
#include "User.h"
class INSTRUCTOR :
	public User
{

	//Attributes

public:

	//Constructor
	INSTRUCTOR();
	INSTRUCTOR(string in_first, string in_last, int in_id);

	//Methods
	void show_classlist();
	void show_schedule();
	void show_search();


	//Destructor
	~INSTRUCTOR();
};


