#include "utils/password_hash.hpp"
#include <iostream>

using namespace std;

int main() {
	auto password = "hello";

	auto hash = PasswordHash::hash(password);

	cout << hash << endl;

	cout << (PasswordHash::verify(password, hash) == true ? "Correct" : "Wrong") << endl;
}