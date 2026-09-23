#pragma once

#define _HAS_STD_BYTE 0

#include "controllers/controller.hpp"
#include "errors/api.hpp"
#include "errors/validation.hpp"
#include "models/students_model.hpp"
#include "dtos/student_dto.hpp"
#include "services/student_service.hpp"
#include <httplib.h>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class StudentController : public IController {
  private:
    std::unique_ptr<StudentService> service;

  public:
    explicit StudentController(std::unique_ptr<StudentService> service)
        : service(std::move(service)) {}

    void index(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);
        std::vector<Student> students = this->service->getUsers(token);

        std::vector<StudentResponse> student_res;
        for(auto student: students) {
            student_res.push_back(StudentResponse(student));
        }

        auto response = StudentsResponse(student_res);

        sendJson(res, 200, response.toJson());
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
