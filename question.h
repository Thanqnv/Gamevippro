#ifndef QUESTION_H
#define QUESTION_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const std::string& fileName, SDL_Renderer* renderer);
    static void DrawTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int width, int height);
    static SDL_Texture* RenderText(const std::string& text, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);
};

#endif // TEXTURE_QUESTION_H
