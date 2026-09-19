#pragma once

#include <string>

using namespace std;

class Cycle {
	public:
		int id;

		Cycle(int id) : id(id) {}

		inline string getName() {
			return string("Cycles:\n\t") + "Id: " + to_string(this->id) + "\n\t"; 
		}
};