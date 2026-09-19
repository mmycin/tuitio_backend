#include <iostream>
#include <sw/redis++/redis++.h>

int main() {
    try {
        sw::redis::Redis redis("tcp://100.80.4.100:6379");

        redis.setex("foo", 5, "bar");

        // auto value = redis.get("hello");

        // if (value) {
        //     std::cout << "Value: " << *value << '\n';
        // } else {
        //     std::cout << "Key not found\n";
        // }
    } catch (const sw::redis::Error& e) {
        std::cerr << "Redis error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
