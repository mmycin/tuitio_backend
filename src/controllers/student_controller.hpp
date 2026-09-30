#pragma once

#define _HAS_STD_BYTE 0

#include "controllers/controller.hpp"
#include "dtos/student_dto.hpp"
#include "models/students_model.hpp"
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
        for (auto student : students) {
            student_res.push_back(StudentResponse(student));
        }

        auto response = StudentsResponse(student_res);

        sendJson(res, 200, response.toJson());
    }

    void show(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);
        int id = this->getIdFromRequest(req);

        Student student = this->service->getStudentById(token, id);

        if (student.id == 0) {
            sendError(res, 404, "Student not found for the user");
        } else {
            auto student_response = StudentResponse(student);
            sendJson(res, 200, student_response.toJson());
        }
    }

    void create(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);

        CreateStudentRequest createReq = json::parse(req.body);
        Student student;
        student.name = createReq.name;
        student.fee = createReq.fee;

        Cycle cycle;
        cycle.class_count = createReq.class_count;

        auto [createdStudent, createdCycle] =
            this->service->createStudent(token, student, cycle);
        if (createdStudent.id == 0) {
            sendError(res, 501, "Can not create student");
        } else {
            sendJson(res, 200, CreateStudentResponse(createdStudent, createdCycle).toJson());
        }
    }

    void update(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);
        int id = this->getIdFromRequest(req);

        UpdateStudentRequest request = json::parse(req.body);

        Student student;
        student.id = id;
        student.name = request.name;
        student.fee = request.fee;

        auto updated_student = this->service->updateStudent(token, student);

        if (updated_student.id == 0) {
            sendError(res, 501, "Can not update student");
        } else {
            auto response = UpdateStudentResponse(updated_student);

            sendJson(res, 200, response.toJson());
        }
    }

    void destroy(const httplib::Request &req, httplib::Response &res) override {
        string token = this->getTokenFromHeader(req);
        int id = this->getIdFromRequest(req);

        bool success = this->service->deleteStudent(token, id);

        if (success) {
            sendJson(res, 200, DeleteStudentResponse(success).toJson());
        } else {
            sendError(res, 501, "Can not delete this student");
        }
    }
};
