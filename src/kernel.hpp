#pragma once

#include <httplib.h>
#include <iostream>
#include "di/user_di.hpp"
#include "routers/user_router.hpp"

using namespace std;

class AppContainer {
	private:
		httplib::Server server;

		UserDI userDi;

		void setUpRoutes() {
			registerUserRoutes(server, *userDi.controller);
		}
	public:
		AppContainer() {
			setUpRoutes();
		}

		void start(const string& host, int port) {
			cout << "Server started at port " << port << endl;
			server.listen(host, port);
		}
};