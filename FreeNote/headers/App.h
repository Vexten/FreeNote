#pragma once
#include "SDL.h"
#undef main

#include "./ILogger.h"

namespace Freenote {

#define FN_ERRORS \
X(APP_SDL_VIDEO_INIT_ERR, "APP_SDL_VIDEO_INIT_ERR") \
X(APP_SDL_WINDOW_CREATE_ERR, "APP_SDL_WINDOW_CREATE_ERR") \
X(APP_GL_CONTEXT_CREATE_ERR, "APP_GL_CONTEXT_CREATE_ERR") \
X(APP_SDL_RENDERER_CREATE_ERR, "APP_SDL_RENDERER_CREATE_ERR") \
X(APP_SDL_ERRORS, "This is used to check for SDL errors, you shouldn't see this text.")


#define X(name, str) name,
	enum APP_ERR : size_t {
		FN_ERRORS
	};
#undef X

#define X(name, str) str,
	constexpr const char* APP_ERR_STR[] = {
		FN_ERRORS
	};
#undef X

#undef FN_ERRORS

	class App
	{
	public:
		static const Uint32 DEFULT_WIDTH = 800;
		static const Uint32 DEFULT_HEIGHT = 600;
		static const Uint32 DEFAULT_FPS = 60;

	private:
		Uint32 frame_time;
		SDL_Window* window;
		SDL_Renderer* renderer;
		bool quit;
		ILogger* logger;
		bool logging;

	public:
		App(Uint32 width, Uint32 height, Uint32 FPS, ILogger* logger);
		App(ILogger* logger) : App(App::DEFULT_WIDTH, App::DEFULT_HEIGHT, App::DEFAULT_FPS, logger) {}
		App() : App(App::DEFULT_WIDTH, App::DEFULT_HEIGHT, App::DEFAULT_FPS, nullptr) {}
		~App();
		void run();

	private:
		void handle_event(SDL_Event* e);
		void render_window();
	};
}