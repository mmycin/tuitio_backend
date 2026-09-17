#pragma once

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using namespace std;

class AppException : public std::runtime_error {
  protected:
    int statusCode;
    nlohmann::json details;

  public:
    AppException(int code, const string &message,
                 nlohmann::json errDetails = nullptr)
        : std::runtime_error(message), statusCode(code), details(errDetails) {}

    int getStatusCode() const {
        return statusCode;
    }

    virtual nlohmann::json toJson() const {
        nlohmann::json json;
        json["status"] = statusCode;
        json["error"] = what();
        if (!details.is_null()) {
            json["details"] = details;
        }
        return json;
    }
};
