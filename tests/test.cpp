#include <iostream>
#include "models/user_model.hpp"
#include "repositories/user_repository.hpp"


using namespace std;

int main() 
{
	UserRepository repo;
	User u = repo.getUserById(4);

	cout << u.getUser() << endl;
    return 0;
}
