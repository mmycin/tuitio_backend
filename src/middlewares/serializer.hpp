#pragma once

#include <httplib.h>

inline void useSerializer(httplib::Server &server) {
    server.set_post_routing_handler([](const httplib::Request &, httplib::Response &res) {
        if (res.get_header_value("Content-Type").empty() && !res.body.empty()) {
            res.set_header("Content-Type", "application/json");
        }
    });
}