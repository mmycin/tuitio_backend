#pragma once

#include "dtos/dto.hpp"
#include "dtos/from_json_macros.hpp"
#include "errors/validation.hpp"
#include "models/classes_model.hpp"
#include "time_utls.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

class CreateClassRequest : public IDTO {
  public:
    int cycle_id = 0;
    string created_at;
    string notes;

    CreateClassRequest() = default;

    CreateClassRequest(int cycle_id, string created_at, string notes) {
        this->cycle_id = cycle_id;
        this->created_at = created_at;
        this->notes = notes;
        validate();
    }

    void validate() override {
        Validator()
            .check(this->cycle_id != 0, "cycle_id", "cycle_id can not be null")
            .check(!this->created_at.empty(), "created_at",
                   "created_at can not be null")
            .check(!this->notes.empty(), "notes", "notes can be empty")
            .validate();
    }

    json toJson() override {
        return json{};
    }
};
FROM_JSON(CreateClassRequest, cycle_id, created_at, notes);

class GetClassByCycleRequest : public IDTO {
  public:
    int cycle_id = 0;

    GetClassByCycleRequest() = default;

    GetClassByCycleRequest(int cycle_id, string created_at, string notes) {
        this->cycle_id = cycle_id;
        validate();
    }

    void validate() override {
        Validator()
            .check(this->cycle_id != 0, "cycle_id", "cycle_id can not be null")
            .validate();
    }

    json toJson() override {
        return json{};
    }
};
FROM_JSON(GetClassByCycleRequest, cycle_id);

class GetClassResponse : public IDTO {
  public:
    Class class_res;

    GetClassResponse() = default;

    GetClassResponse(Class class_res) {
        this->class_res = class_res;
    }

    void validate() override {}

    json toJson() override {
        return json{
            { "success", true },
            {
                "class",
                {
                    { "id", this->class_res.id },
                    { "cycle_id", this->class_res.cycle_id },
                    { "created_at", TimeConverter::timePointToString(
                                        this->class_res.created_at) },
                    { "notes", this->class_res.notes },
                },
            },
        };
    }
};

class GetClassesResponse : public IDTO {
  public:
    std::vector<GetClassResponse> classes;

    GetClassesResponse() = default;

    GetClassesResponse(std::vector<GetClassResponse> classes) {
        this->classes = classes;
    }

    void validate() override {}

    json toJson() override {
        json response;

        response["classes"] = nlohmann::json::array();

        for (auto &class_ : this->classes) {
            response["classes"].push_back(class_.toJson());
        }

        return response;
    }
};

class DeleteClassResponse : public IDTO {
  public:
    bool success;

    DeleteClassResponse() = default;
    DeleteClassResponse(bool success) {
        this->success = success;
    }

    void validate() override {}

    json toJson() override {
        return json{
            { "success", this->success },
        };
    }
};
