#include "repositories/user_repository.hpp"

int main() {
	UserRepository repo;

	auto user = repo.getUserByEmail("mycin.mit@gmail.com");

	cout << user.getUser() << endl;
    return 0;
}
