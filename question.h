#ifndef QUESTION_H
#define QUESTION_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>

class SDL2ImageRenderer {
public:
    SDL2ImageRenderer(SDL_Renderer* renderer) : m_rendererPtr(renderer), m_texturePtr(nullptr), m_width(0), m_height(0) {}
    ~SDL2ImageRenderer() {
        if (m_texturePtr != nullptr) {
            SDL_DestroyTexture(m_texturePtr);
            m_texturePtr = nullptr;
        }
    }

    bool loadImage(const std::string& filePath) {
        SDL_Surface* surfacePtr = IMG_Load(filePath.c_str());
        if (surfacePtr == nullptr) {
            return false;
        }
        m_texturePtr = SDL_CreateTextureFromSurface(m_rendererPtr, surfacePtr);
        if (m_texturePtr == nullptr) {
            SDL_FreeSurface(surfacePtr);
            return false;
        }
        m_width = surfacePtr->w;
        m_height = surfacePtr->h;
        SDL_FreeSurface(surfacePtr);
        return true;
    }

    void renderImage(int x, int y) {
        if (m_texturePtr != nullptr) {
            SDL_Rect destRect = {x, y, m_width, m_height};
            SDL_RenderCopy(m_rendererPtr, m_texturePtr, nullptr, &destRect);
        }
    }

    void setRenderer(SDL_Renderer* renderer) {
        m_rendererPtr = renderer;
    }

    void question2(std:: string &filepath){


    SDL_Window *window2 = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 433, 562, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    setRenderer(renderer2);
    loadImage(filepath.c_str());
    renderImage(0,0);
    SDL_RenderPresent(renderer2);
    SDL_Delay(5000);
    SDL_DestroyWindow(window2);
    SDL_DestroyRenderer(renderer2);
    }

private:
    SDL_Renderer* m_rendererPtr;
    SDL_Texture* m_texturePtr;
    int m_width;
    int m_height;
};

#endif // QUESTION_H
