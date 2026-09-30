#pragma once

#include "dtos/dto.hpp"
#include "dtos/from_json_macros.hpp"
#include "errors/validation.hpp"
#include "models/cycles_model.hpp"
#include <format>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

class CyclesRequest : public IDTO {
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
            .check(this->student_id != 0, "student_id",
                   "Student id can not be null")
            .validate();
    }

    json toJson() override {
        return json{};
    }
};

class GetCycleResponse : public IDTO {
  public:
    Cycle cycle;

    GetCycleResponse() = default;

    GetCycleResponse(Cycle cycle) {
        this->cycle = cycle;
        validate();
    }

    void validate() override {}

    json toJson() override {
        string started_at_str =
            std::format("{:%Y-%m-%d %H:%M:%S}", this->cycle.started_at);

        return json{
            { "id", this->cycle.id },
            { "started_at", started_at_str },
            { "class_count", this->cycle.class_count },
            { "is_paid", this->cycle.is_paid },
        };
    }
};

class GetCyclesResponse : public IDTO {
  public:
    std::vector<GetCycleResponse> cycles;

    GetCyclesResponse() = default;

    GetCyclesResponse(std::vector<GetCycleResponse> cycles) {
        this->cycles = cycles;
    }

    void validate() override {}

    json toJson() override {
        json j;
        j["cycles"] = nlohmann::json::array();

        for (auto &cycle : cycles) {
            j["cycles"].push_back(cycle.toJson());
        }

        return j;
    }
};

FROM_JSON(CyclesRequest, student_id, ids);

class UpdateCycleRequest : public IDTO {
  public:
    bool is_paid;

    UpdateCycleRequest() = default;

    UpdateCycleRequest(bool is_paid) {
        this->is_paid = is_paid;
        validate();
    }

    void validate() override {
        Validator().validate();
    }

    json toJson() override {
        return json{};
    }
};
FROM_JSON(UpdateCycleRequest, is_paid);

class UpdateCycleResponse : public IDTO {
  public:
    Cycle cycle;

    UpdateCycleResponse() = default;

    UpdateCycleResponse(Cycle cycle) {
        this->cycle = cycle;
    }

    void validate() override {}

    json toJson() override {
        return json{
            { "success", true },
            { "cycle", GetCycleResponse(this->cycle).toJson() },
        };
    }
};
