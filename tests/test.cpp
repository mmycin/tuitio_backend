#include "database/database.hpp"
#include "repositories/cycle_repository.hpp"
#include <SQLiteCpp/Exception.h>
#include <iostream>
#include <cassert>

int main() 
{
    try {
        auto &db = getDB();
        db.exec("CREATE TABLE IF NOT EXISTS classes (id INTEGER PRIMARY KEY, cycle_id INTEGER, created_at TEXT, notes TEXT);");
        
        CycleRepository repo;
        int count = repo.getClassCountFromCycleID(1);
        std::cout << "Class count for cycle 1: " << count << std::endl;
        assert(count >= 0);
        std::cout << "Tests passed successfully." << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
