#include "kernel.hpp"
#include <dotenv.h>


int main() {
    dotenv::init();
    AppContainer app;
    app.start("0.0.0.0", 5000);

    return 0;
}
