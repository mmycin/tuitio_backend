#include <dotenv.h>
#include "cache/cache.hpp"

int main() 
{
	dotenv::init();

	Cache::update("name", "bingo", 5 * Cache::Time::Second);
	
    return 0;
}
