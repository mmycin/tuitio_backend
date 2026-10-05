#include "SQLiteCpp/Exception.h"
#include "repositories/cycle_repository.hpp"
#include <SQLiteCpp/Exception.h>
#include <iostream>

using namespace std;

int main() 
{
    CycleRepository repo;

    try {
        int count = repo.getClassCountFromCycleID(1);
        cout << count << endl;
        
    } catch(SQLite::Exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}
