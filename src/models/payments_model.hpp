#pragma once

#include <string>
#include <chrono>

using namespace std;

class Payment {
	public:
		int id;
		int student_id;
		chrono::system_clock::time_point created_at;
		int amount;

		Payment() = default;

		Payment(int id) : id(id) {}

		inline string getName() {
			return string("Payments:\n\t") + "Id: " + to_string(this->id) + "\n\t"; 
		}
};