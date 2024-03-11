#ifndef COLOR_H
#define COLOR_H

#include <SDL.h>

#define RED SDL_Color{255, 0, 0, 255}
#define GREEN SDL_Color{0, 255, 0, 255}
#define BLUE SDL_Color{0, 0, 255, 255}
#define YELLOW SDL_Color{255, 255, 0, 255}
#define ORANGE SDL_Color{255, 140, 0, 255}
#define PINK SDL_Color{255, 0, 255, 255}
#define AQUA SDL_Color{0, 255, 255, 255}
#define WHITE SDL_Color{255, 255, 255, 255}

static bool operator == (const SDL_Color& left, const SDL_Color& right)
{
    return left.a == right.a &&
           left.r == right.r &&
           left.g == right.g &&
           left.b == right.b;
}

#endif // COLOR_H
