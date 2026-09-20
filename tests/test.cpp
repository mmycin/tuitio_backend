#include <dotenv.h>
#include <iostream>
#include "cache.hpp"

int main() 
{
	dotenv::init();

	auto val = Cache::get("name");
	cout << *val << endl;
	
    return 0;
}
