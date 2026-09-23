#pragma once

#include "routers/router.hpp"
#include "controllers/student_controller.hpp"

class StudentRouter : public IRouter {
private:
    StudentController &controller;

public:
    explicit StudentRouter(StudentController &controller) : controller(controller) {}

    void registerRoutes(httplib::Server &server) {
        registerResource(server, "/students", controller);
    }
};
