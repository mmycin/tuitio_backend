#pragma once

#include "dtos/dto.hpp"
#include "models/user_model.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class IAuthDTO : public IDTO {
  public:
    User user;
};

class LoginResponse : public IAuthDTO {
  public:
    string token;

    LoginResponse() = default;

    LoginResponse(string token, User user) {
        this->token = token;
        this->user = user;
        validate();
    }

    void validate() override {
        if (this->token.empty() || this->user.id <= 0 ||
            this->user.name.empty() || this->user.email.empty()) {
            throw std::invalid_argument("Validation Failed: token, id, name, "
                                        "or email is missing/invalid.");
        }
    }


    json toJson() override {
        return json{
            { "token", this->token },
            { "user",
              {
                  { "id", this->user.id },
                  { "name", this->user.name },
                  { "email", this->user.email },
              } },
        };
    }
};
