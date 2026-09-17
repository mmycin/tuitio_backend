#pragma once 

#include <httplib.h>

inline void useCors(httplib::Server &server) {
	server.set_post_routing_handler([](const httplib::Request &req, httplib::Response &res) {
		res.set_header("Access-Control-Allow-Origin", "*");
		res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
	});

	server.Options(R"(.*)", [](const httplib::Request &req, httplib::Response &res) {
		res.status = 204;
	});
}