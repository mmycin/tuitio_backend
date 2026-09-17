#pragma once

#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>
#include "models/user_model.hpp"
#include "dtos/dto.hpp"

using namespace std;

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
            throw std::invalid_argument("Validation Failed: id, name, or email is missing/invalid.");
        }
    }

    static GetUserResponse fromUser(const User& user) {
        return GetUserResponse(user.id, user.name, user.email);
    }

    nlohmann::json toJson() override {
        nlohmann::json j;
        j["id"] = this->id;
        j["name"] = this->name;
        j["email"] = this->email;
        return j;
    }
};