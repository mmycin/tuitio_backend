#include "kernel.hpp"

int main() {
	AppContainer app;
	app.start("0.0.0.0", 5000);

	return 0;
}