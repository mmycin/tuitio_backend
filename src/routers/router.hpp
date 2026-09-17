#pragma once

#include <httplib.h>
#include "controllers/controller.hpp"

class IRouter {
protected:
    void registerResource(httplib::Server &server, const std::string &prefix, IController &controller) {
        
        // GET /prefix (Index)
        server.Get(prefix, [&controller](const httplib::Request &req, httplib::Response &res) {
            controller.index(req, res);
        });

        // GET /prefix/:id (Show)
        server.Get(prefix + R"(/(\d+))", [&controller](const httplib::Request &req, httplib::Response &res) {
            controller.show(req, res);
        });

        // POST /prefix (Create)
        server.Post(prefix, [&controller](const httplib::Request &req, httplib::Response &res) {
            controller.create(req, res);
        });

        // PUT /prefix/:id (Update)
        server.Put(prefix + R"(/(\d+))", [&controller](const httplib::Request &req, httplib::Response &res) {
            controller.update(req, res);
        });

        // DELETE /prefix/:id (Destroy)
        server.Delete(prefix + R"(/(\d+))", [&controller](const httplib::Request &req, httplib::Response &res) {
            controller.destroy(req, res);
        });
    }
};