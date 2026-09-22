#include "repositories/user_repository.hpp"
#include "utils/password_hash.hpp"

int main() {
	UserRepository repo;

	User user("mycin", "mycin.mit@gmail.com", PasswordHash::hash("12345678"));

	repo.createUser(user);
    return 0;
}
