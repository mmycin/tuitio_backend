#pragma once

#include "dtos/dto.hpp"
#include "errors/validation.hpp"
#include "models/user_model.hpp"
#include <nlohmann/json.hpp>
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
        validate();
    }

    void validate() override {
        Validator()
            .check(this->id > 0, "id", "Id must be a positive integer")
            .check(!this->name.empty(), "name", "Name is required")
            .check(!this->email.empty(), "email", "Email is required")
            .validate();
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

class DeleteUserResponse : public IDTO {
  public:
    bool success = false;

    void validate() override {}

    DeleteUserResponse(bool success) {
        this->success = success;
    }

    json toJson() override {
        return json{
            { "success", this->success },
        };
    }
};
