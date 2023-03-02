#include "../headers/Shape.h"
#include <cmath>

#define SHAPE_DEFAULT_MAX_SIZE 100

//TODO: Board (separate screenspace from shape location)
//TODO: Vertex movement on shape rotation

#define VERT_SIGN "v"
#define UV_COORD_SIGN "vt"

//TODO: doesn't react to a non-existant file (what?). Make it do so
const std::vector<SDL_Vertex> Freenote::LoadShape::load_vertex_data_from_obj(std::ifstream& file, SDL_Color color) {
	if (!file.good()) throw; //error reading file
	std::vector<SDL_Vertex> ret;
	std::string s;
	float x, y;
	file.seekg(0, file.beg);
	do file >> s; while (s != VERT_SIGN && !file.eof());
	if (file.eof()) throw; //no vertex data
	while (s == VERT_SIGN) {
		file >> x >> y;
		ret.push_back(SDL_Vertex{ SDL_FPoint{ x, y }, color });
		file >> x;
		file >> s;
	}
	file.seekg(0, file.beg);
	do file >> s; while (s != UV_COORD_SIGN && !file.eof());
	if (file.eof()) {
		for (SDL_Vertex& vert : ret) {
			vert.tex_coord = SDL_FPoint{ -1,-1 };
		}
	}
	else {
		//TODO: throw on malformed uv data
		for (SDL_Vertex& vert : ret) {
			file >> x >> y;
			vert.tex_coord = SDL_FPoint{ x, y };
			file >> s;
		}
	}
	return ret;
}

#undef VERT_SIGN
#undef UV_COORD_SIGN

#define FACE_SIGN "f"

const std::vector<int> Freenote::LoadShape::load_ind_data_from_obj(std::ifstream& file) {
	if (!file.good()) throw; //error reading file
	std::vector<int> ret;
	std::string s;
	int x, y;
	file.seekg(0, file.beg);
	do file >> s; while (s != FACE_SIGN && !file.eof());
	if (file.eof()) throw; //no face data
	while (s == FACE_SIGN) {
		for (int i = 0; i < 3; i++) {
			file >> s;
			int vert_end = s.find_first_of('/');
			//magical -1 because in obj file vertices are 1-indexed
			x = std::stoi(s.substr(0, vert_end)) - 1;
			ret.push_back(x);
		}
		file >> s;
	}
	return ret;
}

#undef FACE_SIGN

/*
Moves shape to target pos
x and y are unknown, write whatever the fuck they turn out to be here (should be center?)
*/
void Freenote::Shape::move(int x, int y) {
	for (SDL_Vertex& vert : this->verts) {
		vert.position.x += x;
		vert.position.y += y;
	}
	this->x = x;
	this->y = y;
}

/*
Currently assumes scaling from [0;0] origin (will that always work?)
*/
void Freenote::Shape::change_scale(float scale) {
	if (scale < 10) return;
	for (SDL_Vertex& vert : this->verts) {
		float* v_x = &(vert.position.x);
		float* v_y = &(vert.position.y);
		*v_x = (*v_x - this->x) * scale / 100.0 + this->x;
		*v_y = (*v_y - this->y) * scale / 100.0 + this->y;
	}
	this->scale = scale;
}

void Freenote::Shape::blow_up() {
	float max_x, max_y, min_x, min_y;
	max_x = this->verts[0].position.x;
	max_y = this->verts[0].position.y;
	min_x = this->verts[0].position.x;
	min_y = this->verts[0].position.y;
	float cx = 0;
	float cy = 0;
	for (const SDL_Vertex& vert : this->verts) {
		cx = vert.position.x;
		cy = vert.position.y;
		if (cx > max_x) max_x = cx;
		else if (cx < min_x) min_x = cx;
		if (cy > max_y) max_y = cy;
		else if (cy < min_y) min_y = cy;
	}
	float x_diff = abs(max_x - min_x);
	float y_diff = abs(max_y - min_y);
	float scale = SHAPE_DEFAULT_MAX_SIZE / ((x_diff > y_diff) ? x_diff : y_diff);
	this->change_scale(scale * 100);
	this->scale = 100;
}

Freenote::Shape::Shape(SDL_Texture* tex, std::vector<SDL_Vertex>* verts, std::vector<int>* indices, int x, int y, float scale) {
	this->tex = tex;
	this->ind = indices;
	//TODO: w h calculation
	this->w = 0; this->h = 0;
	this->verts = std::vector<SDL_Vertex>(*verts);
	this->move(x,y);
	this->blow_up();
	this->change_scale(scale);
	angle = 0.0;
}

void Freenote::Shape::draw(SDL_Renderer* renderer) {
	SDL_RenderGeometry(renderer, tex, verts.data(), verts.size(), (*ind).data(), (*ind).size());
}
