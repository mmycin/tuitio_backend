#pragma once

#include <string>

using namespace std;

class Classe {
	public:
		int id;

		Classe(int id) : id(id) {}

		inline string getName() {
			return string("Classes:\n\t") + "Id: " + to_string(this->id) + "\n\t"; 
		}
};