#include "Application2D.h"

int main() {
	
	// allocation
	auto app = new Application2D();

	// initialise and loop
	app->run("AIE", 1920, 1080, true);

	// deallocation
	delete app;

	return 0;
}