#pragma once

#include <algorithm>
#include <memory>
#include "di/di.hpp"
#include "repositories/student_repository.hpp"
#include "services/student_service.hpp"
#include "controllers/student_controller.hpp"
#include "utils/bearer_token.hpp"
#include "routers/student_router.hpp"

class StudentDI : public IDI {
private:
    std::unique_ptr<StudentRepository> repo;
    std::unique_ptr<StudentService> service;
    std::unique_ptr<StudentController> controller;
    std::unique_ptr<StudentRouter> router;
    std::unique_ptr<AuthManager> auth_manager;

public:
    StudentDI() {
        repo = std::make_unique<StudentRepository>();
        auth_manager = std::make_unique<AuthManager>();
        service = std::make_unique<StudentService>(std::move(repo), std::move(auth_manager));
        controller = std::make_unique<StudentController>(std::move(service));
        router = std::make_unique<StudentRouter>(*controller);
    }

    void registerRoutes(httplib::Server &server) override {
        router->registerRoutes(server);
    }

    IController& getController() override {
        return *controller;
    }

    IService& getService() override {
        return *service;
    }

    IRepository& getRepository() override {
        return *repo;
    }

    IRouter& getRouter() override {
        return *router;
    }
};
