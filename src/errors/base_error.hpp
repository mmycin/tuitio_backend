#pragma once

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using namespace std;
using json = nlohmann::json;

class AppException : public std::runtime_error {
  protected:
    int statusCode;
    json details;

  public:
    AppException(int code, const string &message,
                 json errDetails = nullptr)
        : std::runtime_error(message), statusCode(code), details(errDetails) {}

    int getStatusCode() const {
        return statusCode;
    }

    virtual json toJson() const {
        json json;
        json["status"] = statusCode;
        json["error"] = what();
        if (!details.is_null()) {
            json["details"] = details;
        }
        return json;
    }
};
