#pragma once

#include <nlohmann/json.hpp>
#include "errors/api.hpp"
#include <string>

class Validator {
private:
    nlohmann::json fieldErrors;

public:
    // Add a check condition. If false, record the error for that field.
    Validator& check(bool condition, const std::string &field, const std::string &message) {
        if (!condition) {
            fieldErrors[field] = message;
        }
        return *this;
    }

    // Check if any errors were recorded and throw the global ValidationError
    void validate() {
        if (!fieldErrors.empty()) {
            throw ValidationError(fieldErrors);
        }
    }

    // Static helper for quick validation checks
    static void assertField(bool condition, const std::string &field, const std::string &message) {
        if (!condition) {
            nlohmann::json err;
            err[field] = message;
            throw ValidationError(err);
        }
    }
};