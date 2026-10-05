#include "SQLiteCpp/Exception.h"
#include "repositories/cycle_repository.hpp"
#include <SQLiteCpp/Exception.h>
#include <iostream>

using namespace std;

int main() 
{
    CycleRepository repo;

    try {
        auto oldCycle = repo.getCycleByID(1);
        auto [oldCycleUpdated, newCycle] = repo.splitCycle(oldCycle);

        cout << "Old Cycle:" << endl;
        cout << oldCycleUpdated.id << ", " << oldCycleUpdated.is_paid << endl;
        cout << "New Cycle:" << endl;
        cout << newCycle.id << ", " << newCycle.is_paid << endl;
        
    } catch(SQLite::Exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}
