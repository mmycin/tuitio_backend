#include "utils/bearer_token.hpp"
#include <iostream>
#include "database/database.hpp"
#include <SQLiteCpp/Statement.h>

int main()
{
    AuthManager auth;
    string token;

    auto &db = getDB();

    SQLite::Statement query(db, R"SQL(
		    SELECT password_hash
		    FROM users
		    WHERE id = ?;
    )SQL");
    query.bind(1, 4);

    while(query.executeStep()) {
    	token = query.getColumn(0).getString();
    }

    optional<AuthManager::Id> id = auth.get_id(token);
    if (!id) {
        cout << "Id not fetched" << endl;
    }

    cout << *id << endl;

    return 0;
}
