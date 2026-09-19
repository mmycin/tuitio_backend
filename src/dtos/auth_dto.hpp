#pragma once

#include "dtos/dto.hpp"
#include "dtos/user_dto.hpp"
#include "errors/validation.hpp"
#include "models/user_model.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class IAuthRequestDTO : public IDTO {
  public:
    string email;
    string password;
};

class LoginRequest : public IAuthRequestDTO {
  public:
    LoginRequest() = default;

    LoginRequest(string email, string password) {
        this->email = email;
        this->password = password;
        validate();
    }

    void validate() override {
        Validator()
            .check(!this->email.empty(), "email", "Email is required")
            .check(!this->password.empty(), "password", "Password is required")
            .validate();
    }

    json toJson() override {
        return json{ { "email", this->email }, { "password", this->password } };
    }
};

class SignUpRequest : public IAuthRequestDTO {
  public:
    string name;

    SignUpRequest() = default;

    SignUpRequest(string name, string email, string password) {
        this->name = name;
        this->email = email;
        this->password = password;
        validate();
    }

    void validate() override {
        Validator()
            .check(!this->name.empty(), "name", "Name is required")
            .check(!this->email.empty(), "email", "Email is required")
            .check(!this->password.empty(), "password", "Password is required")
            .validate();
    }

    json toJson() override {
        return json{
            { "name", this->name },
            { "email", this->email },
            { "password", this->password },
        };
    }
};

class IAuthResponseDTO : public IDTO {
  public:
    User user;
};

class LoginResponse : public IAuthResponseDTO {
  public:
    string token;

    LoginResponse() = default;

    LoginResponse(string token, User user) {
        this->token = token;
        this->user = user;
        validate();
    }

    void validate() override {
        Validator()
            .check(!this->token.empty(), "token", "Token is required")
            .check(this->user.id > 0, "user.id", "User id must be a positive integer")
            .check(!this->user.name.empty(), "user.name", "User name is required")
            .check(!this->user.email.empty(), "user.email", "User email is required")
            .validate();
    }

    json toJson() override {
        return json{
            { "success", true },
            { "token", this->token },
        };
    }
};

class SignUpResponse : public IAuthResponseDTO {
  public:
    SignUpResponse() = default;
    SignUpResponse(User user) {
        this->user = user;
        validate();
    }

    void validate() override {
        Validator()
            .check(this->user.id > 0, "user.id", "User id must be a positive integer")
            .check(!this->user.name.empty(), "user.name", "User name is required")
            .validate();
    }

    json toJson() override {
        return GetUserResponse::fromUser(user).toJson();
    }
};

namespace nlohmann {
inline void from_json(const json &j, LoginRequest &req) {
    j.at("email").get_to(req.email);
    j.at("password").get_to(req.password);
    req.validate();
}

inline void from_json(const json &j, SignUpRequest &req) {
    j.at("name").get_to(req.name);
    j.at("email").get_to(req.email);
    j.at("password").get_to(req.password);
    req.validate();
}
} // namespace nlohmann
