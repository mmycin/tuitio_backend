#pragma once

#include "dtos/class_dto.hpp"
#include "time_utls.hpp"
#include <vector>
#define _HAS_STD_BYTE 0

#include "controllers/controller.hpp"
#include "errors/api.hpp"
#include "errors/validation.hpp"
#include "services/class_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>

using namespace std;

class ClassController : public IController {
  private:
    std::unique_ptr<ClassService> service;

  public:
    explicit ClassController(std::unique_ptr<ClassService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        GetClassByCycleRequest request = json::parse(req.body);

        auto classes = this->service->getClassesByCycleID(request.cycle_id);

        std::vector<GetClassResponse> classes_res;

        for (auto &class_ : classes) {
            classes_res.push_back(GetClassResponse(class_));
        }

        sendJson(res, 200, GetClassesResponse(classes_res).toJson());
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        int id = this->getIdFromRequest(req);
        auto class_ = this->service->getClassByID(id);
        if (class_.id == 0) {
            sendError(res, 404, "Can not find class");
        } else {
            sendJson(res, 200, GetClassResponse(class_).toJson());
        }
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        CreateClassRequest request = json::parse(req.body);

        Class inputClass;
        inputClass.cycle_id = request.cycle_id;
        inputClass.created_at =
            TimeConverter::sqliteToTimePoint(request.created_at);
        inputClass.notes = request.notes;

        auto outputClass = this->service->createClass(inputClass);
        if (outputClass.id == 0) {
            sendError(res, 404, "Can not find class");
        } else {
            sendJson(res, 200, GetClassResponse(outputClass).toJson());
        }
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        sendError(res, 405, "Method not allowed");
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        int id = this->getIdFromRequest(req);

        auto is_deleted = this->service->deleteClass(id);

        if (is_deleted) {
            sendJson(res, 200, DeleteClassResponse(is_deleted).toJson());
        } else {
            sendError(res, 501, "can not delete class");
        }
    }
};
