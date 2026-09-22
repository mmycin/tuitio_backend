#pragma once

#include <string>

using namespace std;

class Payment {
	public:
		int id;

		Payment(int id) : id(id) {}

		inline string getName() {
			return string("Payments:\n\t") + "Id: " + to_string(this->id) + "\n\t"; 
		}
};