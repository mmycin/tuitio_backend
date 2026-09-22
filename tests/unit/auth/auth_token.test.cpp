#include "utils/bearer_token.hpp"
#include <iostream>

int main()
{
    AuthManager auth;
    string token = auth.make_token(123);

    cout << token << endl;

    optional<AuthManager::Id> id = auth.get_id(token);
    if (!id) {
        cout << "Id not fetched" << endl;
    }

    cout << *id << endl;

    return 0;
}
