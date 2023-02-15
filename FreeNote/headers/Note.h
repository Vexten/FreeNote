#pragma once
#include "Shape.h"

#define NTC SDL_Color{ 255, 255, 0, 255 }
#define TEST_W 100
#define TEST_H TEST_W

namespace NOTE {
    const std::vector<SDL_Vertex> base_verts{
        { SDL_FPoint{ 0, 30 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 50, 0 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 100, 30 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 150, 0 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 150, 30 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 100, 60 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 50, 30 }, NTC, SDL_FPoint{0} },
        { SDL_FPoint{ 0, 60 }, NTC, SDL_FPoint{0} },
    };

    const std::vector<int> indices = calculate_indices(&base_verts);
}

class Note :
    public Shape
{
    /*
    0--1
    |  |
    3--2
    */

public:
    Note(int x, int y) : Shape(
        &NOTE::base_verts,
        NOTE::indices,
        x,
        y,
        50
    ) {}
};

