#pragma once
#include "SDL.h"
#undef main

#include "ILogger.h"
#include "Errors.h"
#include "RecourceStorage.h"
#include "Shape.h"

namespace Freenote {

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
		ResourceStorage* r_storage;
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