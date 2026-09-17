#pragma once

#include <string>
#include "models/user_model.hpp"

using namespace std;

class Student {
	public:
		int id;
		User user;
		string name;
		int fee;

		Student(int id) : id(id), user(0) {}
		Student(int id, User user, string name, int fee) : id(id), user(user), name(name), fee(fee) {}

		inline string getName() {
			return string("Students:\n\t") + "Id: " + to_string(this->id) + "\n\t"; 
		}
};