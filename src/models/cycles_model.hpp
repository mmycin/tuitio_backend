#pragma once

#include <string>
#include <chrono>

using namespace std;

class Cycle {
	public:
		int id = 0;
		int student_id;
		chrono::system_clock::time_point started_at;
		int class_count;
		bool is_paid;
		

		Cycle() = default;

		Cycle(int id) : id(id) {}

		inline string getName() {
			return string("Cycles:\n\t") + "Id: " + to_string(this->id) + "\n\t"; 
		}
};