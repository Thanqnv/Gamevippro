#include <iostream>
#include "Loadimage.h"
#include <SDL.h>
#include <SDL_image.h>

std::string imageFiles[NUM_IMAGES] = {
    "a.png", "b.png", "c.png", "d.png", "e.png", "f.png", "g.png", "h.png", "i.png", "k.png", "l.png", "m.png",
    "n.png", "o.png", "p.png", "q.png", "r.png", "s.png", "t.png", "u.png", "v.png", "w.png", "x.png", "y.png"};

std::string imageDir = "image/chu/";



void runSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    int imgFlags = IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Surface* backgroundSurface = IMG_Load("image/background.jpg");
    if (backgroundSurface == nullptr) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* cauhoiSurface = IMG_Load("image/cauhoi/cau1.jpg");
    if (cauhoiSurface == nullptr) {
        std::cerr << "Unable to load image from image/cauhoi! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* cauhoiTexture = SDL_CreateTextureFromSurface(renderer, cauhoiSurface);
    SDL_FreeSurface(cauhoiSurface);

    SDL_Surface* deSurface = IMG_Load("image/de.png");
    if (deSurface == nullptr) {
        std::cerr << "Unable to load image from image! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* deTexture = SDL_CreateTextureFromSurface(renderer, deSurface);
    SDL_FreeSurface(deSurface);

    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect cauhoiRect = {110, 100, 500, 400};
    SDL_Rect deRect = {105, 490, 522, 200};
    SDL_Rect newImageRect = {150, 450, IMAGE_SIZE, IMAGE_SIZE};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);
    SDL_RenderCopy(renderer, cauhoiTexture, nullptr, &cauhoiRect);
    SDL_RenderCopy(renderer, deTexture, nullptr, &deRect);

    SDL_Texture* textures[NUM_IMAGES];
    SDL_Rect destRect[NUM_IMAGES];
    int x = 125, y = 560;

    for (int i = 0; i < NUM_IMAGES; ++i) {
        std::string imagePath = imageDir + imageFiles[i];
        SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
        if (imageSurface == nullptr) {
            std::cerr << "Failed to load image: " << imagePath << std::endl;
            continue;
        }

        SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, IMAGE_SIZE, IMAGE_SIZE, 32, 0, 0, 0, 0);
        SDL_Rect stretchRect = {0, 0, IMAGE_SIZE, IMAGE_SIZE};
        SDL_BlitScaled(imageSurface, nullptr, resizedSurface, &stretchRect);

        textures[i] = SDL_CreateTextureFromSurface(renderer, resizedSurface);
        SDL_FreeSurface(imageSurface);
        SDL_FreeSurface(resizedSurface);

        if (textures[i] != nullptr) {
            SDL_QueryTexture(textures[i], nullptr, nullptr, &destRect[i].w, &destRect[i].h);
            destRect[i].x = x;
            destRect[i].y = y;
            SDL_RenderCopy(renderer, textures[i], nullptr, &destRect[i]);

            x += IMAGE_SIZE + 10;
            if (x >= 600 - IMAGE_SIZE) {
                x = 125;
                y += IMAGE_SIZE + 10;
            }
        }
    }

    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.clicks == 2) {
                for (int i = 0; i < NUM_IMAGES; ++i) {
                    if (event.button.x >= destRect[i].x && event.button.x <= destRect[i].x + destRect[i].w &&
                        event.button.y >= destRect[i].y && event.button.y <= destRect[i].y + destRect[i].h) {
                        SDL_RenderCopy(renderer, textures[i], nullptr, &newImageRect);
                        break;
                    }
                }
                SDL_RenderPresent(renderer);
            }
        }
    }

    for (int i = 0; i < NUM_IMAGES; ++i) {
        if (textures[i] != nullptr) {
            SDL_DestroyTexture(textures[i]);
        }
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(cauhoiTexture);
    SDL_DestroyTexture(deTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
