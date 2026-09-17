#pragma once

#include <memory>
#include <httplib.h>
#include "nlohmann/json.hpp"
#include "services/user_service.hpp"
#include "dtos/user_dto.hpp"
#include <string>

using namespace std;

class UserController {
private:
    std::unique_ptr<UserService> service;

public:
    explicit UserController(std::unique_ptr<UserService> service) 
        : service(std::move(service)) {}

    void show(const httplib::Request &req, httplib::Response &res) {
        try {
            string id_str = req.matches[1];
            int userId = std::stoi(id_str);

            auto user = service->getUserById(userId);
            auto user_dao = GetUserResponse::fromUser(user);

            nlohmann::json response;
            response["status"] = 200;
            response["user"] = user_dao.toJson();

            res.set_content(response.dump(), "application/json");
        } catch (const exception &e) {
            nlohmann::json error;
            error["status"] = 400;
            error["error"] = e.what();
            res.status = 400;
            res.set_content(error.dump(), "application/json");
        }
    }
};