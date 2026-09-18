#pragma once

#include "dtos/dto.hpp"
#include "models/user_model.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>


using namespace std;
using json = nlohmann::json;

class IUserDTO : public IDTO {
  public:
    int id;
    string name;
    string email;
};

class GetUserResponse : public IUserDTO {
  public:
    GetUserResponse() = default;

    GetUserResponse(int id, string name, string email) {
        this->id = id;
        this->name = name;
        this->email = email;
        validate(); // Ensures data is valid upon creation
    }

    void validate() override {
        if (this->id <= 0 || this->name.empty() || this->email.empty()) {
            throw std::invalid_argument(
                "Validation Failed: id, name, or email is missing/invalid.");
        }
    }

    static GetUserResponse fromUser(const User &user) {
        return GetUserResponse(user.id, user.name, user.email);
    }

    json toJson() override {
        return json{
            { "id", this->id },
            { "name", this->name },
            { "email", this->email },
        };
    }
};
