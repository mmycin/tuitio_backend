#include "kernel.hpp"
#include <dotenv.h>
#include "configs/app_config.hpp"

int main() {
    dotenv::init();
    AppContainer app;
    app.start(AppConfig::APP_HOST, AppConfig::APP_PORT);

    return 0;
}
