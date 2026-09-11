#include "database/database.hpp"
#include <SQLiteCpp/Statement.h>
#include <iostream>

using namespace std;

int main()
{
    try {
        auto &db = getDB();

        SQLite::Statement query(db, R"SQL(
	    SELECT id, name, email
	    FROM users
	    WHERE id = ?;
    )SQL");
        query.bind(1, 15);

        while (query.executeStep()) {
            int id = query.getColumn(0);
            string name = query.getColumn(1);
            string email = query.getColumn(2);

            cout << "ID: " << id << ", Name: " << name << ", Email: " << email
                 << endl;
        }
    } catch (std::exception &e) {
        cout << "exception: " << e.what() << endl;
    }

    return 0;
}
