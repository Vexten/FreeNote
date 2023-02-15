#include "Shape.h"
#include <cmath>

//TODO: Board (separate screenspace from shape location)
//TODO: Vertex movement on shape scale
//TODO: Vertex movement on shape rotation

constexpr float sign(const SDL_Vertex* p, const SDL_Vertex* a, const SDL_Vertex* b)
{
	return (p->position.x - b->position.x) * (a->position.y - b->position.y) - (a->position.x - b->position.x) * (p->position.y - b->position.y);
}

//is v inside of abc
constexpr bool is_inside_triangle(const SDL_Vertex* v, const SDL_Vertex* a, const SDL_Vertex* b, const SDL_Vertex* c) {
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(v, a, b);
	d2 = sign(v, b, c);
	d3 = sign(v, c, a);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

constexpr int is_clockwise(const SDL_Vertex* a, const SDL_Vertex* b, const SDL_Vertex* c) {
	float p1 = (b->position.x - a->position.x) * (c->position.y - a->position.y);
	float p2 = (c->position.x - a->position.x) * (b->position.y - a->position.y);
	float val = p1 - p2;
	return (0.0f < val) - (val < 0.0f);
}

const std::vector<int> calculate_indices(const std::vector<SDL_Vertex>* verts)
{
	int v_len = verts->size();
	const SDL_Vertex* v_data = verts->data();
	std::vector<int> t_v(v_len);
	for (int i = 0; i < v_len; i++) {
		t_v[i] = i;
	}
	std::vector<int> ind;
	int max_ind = 0;
	int max_x = v_data[0].position.x;
	for (int i = 0; i < v_len; i++) {
		if (v_data[i].position.x > max_x) {
			max_x = v_data[i].position.x;
			max_ind = i;
		}
	}
	const SDL_Vertex* b = &v_data[max_ind];
	int ai = max_ind - 1;
	if (ai < 0) ai = v_len;
	int ci = max_ind + 1;
	if (ci > (v_len - 1)) ci = 0;
	const SDL_Vertex* a = &v_data[ai];
	const SDL_Vertex* c = &v_data[ci];
	int poly_cw = is_clockwise(a, b, c);
	int i = 0;
	while (t_v.size() > 3) {
		bool contains_vertex = false;
		int ind1 = t_v[i % t_v.size()];
		int ind2 = t_v[(i + 1) % t_v.size()];
		int ind3 = t_v[(i + 2) % t_v.size()];
		const SDL_Vertex* p1 = &v_data[ind1];
		const SDL_Vertex* p2 = &v_data[ind2];
		const SDL_Vertex* p3 = &v_data[ind3];
		if (poly_cw == is_clockwise(p1, p2, p3)) {
			for (int j = 0; j < v_len; j++) {
				if ((j == ind1) || (j == ind2) || (j == ind3)) continue;
				if (is_inside_triangle(&v_data[j], p1, p2, p3)) {
					contains_vertex = true;
					break;
				}
			}
			if (contains_vertex) {
				i++;
				continue;
			}
			ind.push_back(ind1);
			ind.push_back(ind2);
			ind.push_back(ind3);
			t_v.erase(t_v.begin() + ((i + 1) % t_v.size()));
		}
		else {
			i++;
		}
	}
	for (int i : t_v) {
		ind.push_back(i);
	}
	return ind;
}

/*
Moves shape to target pos
x and y are unknown, write whatever the fuck they turn out to be here (should be center?);
*/
void Shape::move_shape(int x, int y) {
	int v_len = this->verts.size();
	SDL_Vertex* v_data = this->verts.data();
	for (int i = 0; i < v_len; i++) {
		v_data[i].position.x += x;
		v_data[i].position.y += y;
	}
	this->x = x;
	this->y = y;
}

/*
Currently assumes scaling from [0;0] origin (will that always work?)
*/
void Shape::scale_shape(int scale) {
	if (scale < 10) return;
	int v_len = this->verts.size();
	SDL_Vertex* v_data = this->verts.data();
	for (int i = 0; i < v_len; i++) {
		float* v_x = &(v_data[i].position.x);
		float* v_y = &(v_data[i].position.y);
		*v_x = (*v_x - this->x) * scale / 100.0 + this->x;
		*v_y = (*v_y - this->y) * scale / 100.0 + this->y;
	}
	this->scale = scale;
}

Shape::Shape(SDL_Texture* tex, const std::vector<SDL_Vertex>* verts, const std::vector<int> indices, int x, int y, float scale) {
	this->tex = tex;
	this->ind = indices;
	//TODO: w h calculation
	this->w = 0; this->h = 0;
	this->verts = std::vector(*verts);
	this->move_shape(x,y);
	this->scale_shape(scale);
	angle = 0.0;
}

void Shape::draw(SDL_Renderer* renderer) {
	SDL_RenderGeometry(renderer, tex, verts.data(), verts.size(), ind.data(), ind.size());
}
