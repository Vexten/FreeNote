#pragma once
#include "SDL.h"
#include <vector>
#include <string>

const std::vector<int> calculate_indices(const std::vector<SDL_Vertex>* verts);

class Shape
{
private:
	SDL_Texture* tex;
	std::vector<SDL_Vertex> verts;
	std::vector<int> ind;
	int x, y;
	int w, h;
	float angle;
	int scale;

	void move_shape(int x, int y);
	void scale_shape(int scale);

public:
	std::string text;

	Shape(SDL_Texture* tex, const std::vector<SDL_Vertex>* verts, const std::vector<int> indices, int x, int y, float scale);
	Shape(const std::vector<SDL_Vertex>* verts, const std::vector<int> indices, int x, int y, float scale) : Shape(nullptr, verts, indices, x, y, scale) {}

	void draw(SDL_Renderer* renderer);
};
