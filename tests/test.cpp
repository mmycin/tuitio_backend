#include "utils/bearer_token.hpp"
#include <iostream>

int main()
{
	AuthManager auth;
    uint64_t userId = 42;
	
    // 1. Generate token (valid for 3 hours)
    std::string token = auth.make_token(userId);
    std::cout << "Generated Token: " << token << "\n";
	
    // 2. Validate token and get user ID back
    auto id = auth.get_id(token);
    if (id) {
        std::cout << "Token valid! User ID: " << *id << "\n";
    } else {
        std::cout << "Token invalid or expired.\n";
    }

    return 0;
}
