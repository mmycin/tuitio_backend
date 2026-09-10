#include "database/database.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        std::cout << "Before DB\n";

        auto& db = getDB();

        std::cout << "DB initialized successfully\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "UNKNOWN EXCEPTION\n";
        return 2;
    }
}
