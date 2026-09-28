#pragma once

#include <vector>
#define _HAS_STD_BYTE 0

#include "controllers/controller.hpp"
#include "dtos/cycle_dto.hpp"
#include "errors/api.hpp"
#include "errors/validation.hpp"
#include "services/cycle_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>

using namespace std;

class CycleController : public IController {
  private:
    std::unique_ptr<CycleService> service;

  public:
    explicit CycleController(std::unique_ptr<CycleService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        CyclesRequest request = json::parse(req.body);

        auto cycles =
            this->service->getCyclesByIDs(request.ids, request.student_id);

        std::vector<GetCycleResponse> cycles_res;

        for (auto cycle : cycles) {
            cycles_res.push_back(GetCycleResponse(cycle));
        }

        auto cycles_response = GetCyclesResponse(cycles_res);

        sendJson(res, 200, cycles_response.toJson());
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        string id_str = req.matches[1];
        Validator::assertField(!id_str.empty(), "id", "Id is required");

        int itemId = 0;
        try {
            itemId = std::stoi(id_str);
        } catch (const std::exception &) {
            nlohmann::json details;
            details["id"] =
                nlohmann::json::array({ "Id must be a valid integer" });
            throw ValidationError(details, "Validation failed");
        }

        sendError(res, 405, "Method not implemented");
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }
};
