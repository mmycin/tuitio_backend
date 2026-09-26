#pragma once

#include "dtos/dto.hpp"
#include "errors/validation.hpp"
#include "dtos/from_json_macros.hpp"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CyclesRequest : IDTO {
    public:
        int student_id = 0;
        std::vector<int> ids;
        
        CyclesRequest() = default;

        CyclesRequest(int student_id, std::vector<int> ids) {
            this->student_id = student_id;
            this->ids = ids;
            validate();
        }

        void validate() override {
            Validator()
                .check(this->student_id != 0, "student_id", "Student id can not be null")
                .validate();
        }

        json toJson() override {
            return json{};
        }
        
};

FROM_JSON(CyclesRequest, student_id, ids);