#include <iostream>
#include "utils/env.hpp"

int main() 
{
	cout << getEnv("SECRET_KEY") << endl;
	return 0;
}
