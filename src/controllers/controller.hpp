#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>

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
                  const nlohmann::json &data) {
        nlohmann::json response;
        response["status"] = status;
        response["data"] = data;
        res.status = status;
        res.set_content(response.dump(), "application/json");
    }

    void sendError(httplib::Response &res, int status,
                   const std::string &message) {
        nlohmann::json error;
        error["status"] = status;
        error["error"] = message;
        res.status = status;
        res.set_content(error.dump(), "application/json");
    }
};
