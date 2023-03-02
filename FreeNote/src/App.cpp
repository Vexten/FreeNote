#include "../headers/App.h"

using namespace Freenote;

std::vector<Shape*> shapes;

#undef FN_ERRORS

#define FN_LOG(x) if (logging) logger->write(x);
#define CTOR_ERR(x) SDL_Quit(); throw x

App::App(Uint32 width, Uint32 height, Uint32 FPS, ILogger* logger) {
	window = NULL;
	renderer = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		CTOR_ERR(APP_SDL_VIDEO_INIT_ERR);
	}
	window = SDL_CreateWindow("FreeNotes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL) {
		CTOR_ERR(APP_SDL_WINDOW_CREATE_ERR);
	}
	// \/ stolen, no clue why this is here.
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	}
	if (SDL_GL_CreateContext(window) == NULL) {
		CTOR_ERR(APP_GL_CONTEXT_CREATE_ERR);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		CTOR_ERR(APP_SDL_RENDERER_CREATE_ERR);
	}
	char* path = SDL_GetBasePath();
	if (path == NULL) {
		CTOR_ERR(APP_SDL_BASE_FOLDER_NOT_RETURNED);
	}
	//may break because of UTF-8, lets hope it doesn't
	r_storage = new ResourceStorage(std::string(path));
	SDL_free(path);
	frame_time = 1000 / FPS;
	quit = false;
	this->logger = logger;
	logging = (this->logger == nullptr) ? false : true;
}

App::~App() {
	FN_LOG("Destroying app.");
	for (Shape* shape : shapes) {
		delete shape;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	delete r_storage;
	SDL_Quit();
}

void App::run() {
	while (!quit) {
		Uint32 start = SDL_GetTicks();
		SDL_Event curr_event = { 0 };
		while (SDL_PollEvent(&curr_event)) {
			handle_event(&curr_event);
		}
		render_window();
		Uint32 dur = SDL_GetTicks() - start;
		if (dur < frame_time) {
			SDL_Delay(frame_time - dur);
		}
	}
}

void App::handle_event(SDL_Event* e) {
	switch (e->type) {
	case SDL_QUIT:
		FN_LOG("Quitting application.");
		quit = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (e->button.button) {
		case SDL_BUTTON_LEFT:
			FN_LOG(std::format("M1_DOWN event (x: {}; y: {})",e->button.x, e->button.y));
			break;
		case SDL_BUTTON_RIGHT:
			FN_LOG(std::format("M2_DOWN event (x: {}; y: {}), creating dummy Note", e->button.x, e->button.y));
			shapes.push_back(new Shape(
				(*r_storage)["shapes"]["arrow"].shape_verts,
				(*r_storage)["shapes"]["arrow"].shape_indices,
				e->button.x,
				e->button.y,
				100.0f));
			break;
		}
		break;
	}
}

void App::render_window() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	for (Shape* shape : shapes) {
		shape->draw(renderer);
	}
	SDL_RenderPresent(renderer);
}

#undef FN_LOG
