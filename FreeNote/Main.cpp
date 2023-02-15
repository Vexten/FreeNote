#include <iostream>
#include "App.h"
#include "ConsoleLogger.h"

using namespace Freenote;

int main(void) {
	ConsoleLogger cl = ConsoleLogger();
	App* app = nullptr;
	try {
		app = new App(&cl);
	}
	catch (APP_ERR e) {
		std::cerr << "Error while initializing application: " << APP_ERR_STR[e] << std::endl;
		if (e < APP_ERR::APP_SDL_ERRORS) {
			std::cerr << "SDL error: " << SDL_GetError() << std::endl;
		}
	}
	if (app != nullptr) {
		app->run();
		delete app;
	}
	return 0;
}