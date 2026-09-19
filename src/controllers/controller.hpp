#pragma once

#include "errors/base_error.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class IController {
  public:
    virtual ~IController() = default;

    virtual void index(const httplib::Request &req, httplib::Response &res) = 0;
    virtual void show(const httplib::Request &req, httplib::Response &res) = 0;
    virtual void create(const httplib::Request &req, httplib::Response &res) = 0;
    virtual void update(const httplib::Request &req, httplib::Response &res) = 0;
    virtual void destroy(const httplib::Request &req, httplib::Response &res) = 0;

  protected:
    void sendJson(httplib::Response &res, int status,
                  const json &data) {
        json response;
        response["status"] = status;
        response["data"] = data;
        res.status = status;
        res.set_content(response.dump(), "application/json");
    }

    void sendError(httplib::Response &res, int status,
                   const std::string &message,
                   const json &details = nullptr) {
        json error;
        error["status"] = status;
        error["error"] = message;
        if (!details.is_null()) {
            error["details"] = details;
        }
        res.status = status;
        res.set_content(error.dump(), "application/json");
    }

    void sendAppError(httplib::Response &res, const AppException &e) {
        res.status = e.getStatusCode();
        res.set_content(e.toJson().dump(), "application/json");
    }
};
