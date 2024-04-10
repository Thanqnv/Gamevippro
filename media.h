#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <cmath>

// (struct for loading and storing graphics/sounds and generating animation clips)struct để tải và lưu trữ đồ họa/âm thanh và tạo các clip hoạt hình
struct Media
{
    //textures
    //active entities
    SDL_Texture *playerTex, *dustTex, *doorTex, *keyTex;
    //map entities
    SDL_Texture *starTex, brickFloorTex, *brickBackgroundTex, *windowTex, *tutorialTex;

    //animation clips
    std::vector<std::vector<SDL_Point>> playerClips, dustClips, doorClips, keyClips;

    //sounds
    //music
    Mix_Music *bgMusic;
    //sfx
    Mix_Chunk *jumpSfx, *thudSfx, *dashSfx, *keySfx, *doorSfx;

    SDL_Renderer *renderer;

    Media(SDL_Renderer *renderer);

    //Load png, return rexture pointer
    SDL_Texture *loadTexture(const char *filePath);
    //Load wav files
    Mix_Music *loadMusic(const char *filePath);
    Mix_Chunk *loadSfx(const char *filePath);

    //make a vector of points representing the top left corner of each animation frame
    //tạo một vectơ các điểm biểu thị góc trên bên trái của mỗi khung hình động
    std::vector<SDL_Point> makeClips(int animNo, int frames, int w, int h);
};
