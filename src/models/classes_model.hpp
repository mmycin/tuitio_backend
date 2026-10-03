#pragma once

#include <chrono>
#include <string>

using namespace std;

class Class {
  public:
    int id = 0;
    int cycle_id = 0;
    chrono::system_clock::time_point created_at;
    string notes;

    Class() = default;
    Class(int id) : id(id) {}

    inline string getName() {
        return string("Classes:\n\t") + "Id: " + to_string(this->id) + "\n\t";
    }
};
