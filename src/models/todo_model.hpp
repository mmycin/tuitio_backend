#pragma once

#include <models/user_model.hpp>
#include <string>

using namespace std;

class Todo {
  public:
    int id;
    User user;
    string title;
    string description;
    bool completed;

    Todo(int id, User user, string title, string description, bool completed)
        : id(id), user(user), title(title), description(description),
          completed(completed) {}

    string getTodo();

  private:
    int user_id;
};

inline string Todo::getTodo() {
    return string("Todo:\n\t") + "Id: " + to_string(this->id) + "\n\t" +
           "Title: " + this->title + "\n\t" +
           "Description: " + this->description + "\n\t" +
           "Completed: " + (this->completed ? "True" : "False") + "\n\t" +
           this->user.getUser();
}
