#pragma once

#include <string>

using namespace std;

class User {
  public:
    int id;
    string name;
    string email;

    User(int id) : id(id) {}

    User(int id, string name, string email, string password_hash)
        : id(id), name(name), email(email), password_hash(password_hash) {}

    string getUser();

  private:
    string password_hash;
};

inline string User::getUser() {
    return string("User:\n\t") + "Id: " + to_string(this->id) + "\n\t" +
           "Name: " + this->name + "\n\t" + "Email: " + this->email;
}
