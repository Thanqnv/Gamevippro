#ifndef PATH_H
#define PATH_H

#include "GameObj.h"

// con duong noi mau
class Path: public GameObj
{
public:
    Path(const unsigned x, const unsigned y, const SDL_Color& c);
    type getType() const;
    void draw(SDL_Renderer* renderer);
};

#endif
