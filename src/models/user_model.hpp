#pragma once

#include <string>

using namespace std;

class User {
  public:
    int id = 0;
    string name;
    string email;
    string password_hash;

    User() = default;

    User(int id) : id(id) {}

    User(string name, string email, string password_hash)
        : name(name), email(email), password_hash(password_hash) {}

    string getUser();
};

inline string User::getUser() {
    return string("User:\n\t") + "Id: " + to_string(this->id) + "\n\t" +
           "Name: " + this->name + "\n\t" + "Email: " + this->email;
}
