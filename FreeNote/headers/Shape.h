#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <fstream>
#include "Errors.h"

namespace Freenote {

	namespace LoadShape {

		const std::vector<SDL_Vertex> load_vertex_data_from_obj(std::ifstream& file, SDL_Color color);

		const std::vector<int> load_ind_data_from_obj(std::ifstream& file);
	}

	class Shape
	{
	private:
		SDL_Texture* tex;
		std::vector<SDL_Vertex> verts;
		std::vector<int>* ind;
		int x, y;
		int w, h;
		float angle;
		float scale;

		void move(int x, int y);
		void change_scale(float scale);
		void blow_up();

	public:
		std::string text;

		Shape(SDL_Texture* tex, std::vector<SDL_Vertex>* verts, std::vector<int>* indices, int x, int y, float scale);
		Shape(std::vector<SDL_Vertex>* verts, std::vector<int>* indices, int x, int y, float scale) : Shape(nullptr, verts, indices, x, y, scale) {}

		void draw(SDL_Renderer* renderer);
	};
}