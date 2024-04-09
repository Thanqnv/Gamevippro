#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "cnst.hpp"
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class BaseObject
{
public:
    BaseObject();
    ~BaseObject();
    void SetRect(const int& x, const int& y) { rect.x = x, rect.y = y; }
    SDL_Rect GetRect() const { return rect; }
    SDL_Texture* GetObject() const { return p_object; };

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();

private:
    SDL_Rect rect;
    SDL_Texture* p_object;
};

#endif // BASE_OBJECT_H
