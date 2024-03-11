#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Color.h>

const int pos = 45;
const int offset = 70;
const int r = 32;
const int gap = 64;

using namespace std;
enum type {BALL, PATH, EMPTY};

class GameObj
{
protected:
    unsigned source_x, source_y;
    SDL_Color color;

public:
    GameObj (const unsigned x, const unsigned y, const SDL_Color& c): source_x(x), source_y(y), color(c){}
    unsigned getX() const {return source_x;}
    unsigned getY() const {return source_y;}

    unsigned getLogicX() const {return (int(source_x - gap)/offset);}
    unsigned getLogicY() const {return (int(source_y - gap)/offset);}

    SDL_Color getColor() const {return color;}
    virtual type getType() const = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;

    bool {
        return {
            this->getLogicX() == g->getLogicX() &&
            this->getLogicY() == g->getLogicY() &&
            this->getColor().r == g->getColor().r &&
            this->getColor().g == g->getColor().g &&
            this->getColor().b == g->getColor().b };
        }
    }
};
#endif // GAMEOBJ_H
