#pragma once
#include "User.h"
class ADMINISTRATOR :
	public User
{
	//Attributes


public:

	//Constructor
	ADMINISTRATOR();
	ADMINISTRATOR(string in_first, string in_last, int in_id);


	//Methods
	void add_remove_course();
	void add_remove_user();
	void add_remove_student();
	void search_print_courses();


	//Destructor
	~ADMINISTRATOR();
};

