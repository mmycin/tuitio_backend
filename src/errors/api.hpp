#pragma once

#include "errors/base_error.hpp"
#include <string>

using namespace std;

class ApiError : public AppException {
  public:
    explicit ApiError(const string &message, int code = 400)
        : AppException(code, message) {}
};

class UnauthorizedError : public AppException {
  public:
    explicit UnauthorizedError(const std::string &message = "Unauthorized")
        : AppException(401, message) {}
};

class NotFoundError : public AppException {
  public:
    explicit NotFoundError(const std::string &message = "Resource not found")
        : AppException(404, message) {}
};

class ValidationError : public AppException {
  public:
    explicit ValidationError(const nlohmann::json &fieldErrors,
                             const std::string &message = "Validation failed")
        : AppException(422, message, fieldErrors) {}
};
