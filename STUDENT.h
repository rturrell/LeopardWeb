#pragma once
#include "User.h"
class STUDENT :
	public User
{
	//Attributes

public:

	//Constructor
	STUDENT();
	STUDENT(string in_first, string in_last, int in_id);

	//Methods

	void search();
	void add_drop();
	void prints();

	//Destructor
	~STUDENT();
};


