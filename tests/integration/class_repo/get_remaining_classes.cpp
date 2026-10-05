#include "SQLiteCpp/Exception.h"
#include "repositories/class_repository.hpp"
#include <SQLiteCpp/Exception.h>
#include <iostream>

using namespace std;

int main() 
{
    ClassRepository repo;

    try {
        auto classes = repo.getRemainingClasses(1, 4);
        for(auto& class_: classes) {
            cout << class_.id << ", " << class_.cycle_id << endl; 
        }
    } catch(SQLite::Exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}
