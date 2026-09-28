#include <chrono>
#include <format>
#include <iostream>

int main() {
    auto now = std::chrono::system_clock::now();
    std::string now_str = std::format("{:%Y-%m-%d %H:%M:%S}", now);

    std::cout << now_str << std::endl;
    return 0;
}
