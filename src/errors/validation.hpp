#pragma once

#include <nlohmann/json.hpp>
#include "errors/api.hpp"
#include <string>

class Validator {
private:
    nlohmann::json fieldErrors;

public:
    Validator& check(bool condition, const std::string &field, const std::string &message) {
        if (!condition) {
            if (!fieldErrors.contains(field)) {
                fieldErrors[field] = nlohmann::json::array();
            }
            fieldErrors[field].push_back(message);
        }
        return *this;
    }

    void validate() {
        if (!fieldErrors.empty()) {
            throw ValidationError(fieldErrors);
        }
    }

    bool hasErrors() const {
        return !fieldErrors.empty();
    }

    const nlohmann::json& getErrors() const {
        return fieldErrors;
    }

    static void assertField(bool condition, const std::string &field, const std::string &message) {
        if (!condition) {
            nlohmann::json err;
            err[field] = nlohmann::json::array({message});
            throw ValidationError(err);
        }
    }
};