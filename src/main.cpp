#include <httplib.h>
#include <nlohmann/json.hpp>

const int PORT = 5000;

int main() {
  httplib::Server server;
  nlohmann::json j;
  j["status"] = 200;
  j["data"] = {{"message", "success"}};

  server.Get("/", [&j](const httplib::Request &req, httplib::Response &res) {
    res.set_content(j.dump(), "application/json");
  });

  server.Get("/hello", [](const httplib::Request &req, httplib::Response &res) {
    res.set_content("world", "text/plain");
  });

  std::cout << "Server started at port: " << PORT << std::endl;
  server.listen("0.0.0.0", PORT);
}
