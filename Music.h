#ifndef MUSIC_H
#define MUSIC_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "iostream"

using namespace std;

class Music
{
private:
    // Nhac nen che do
    Mix_Music* menu;
    Mix_Music* game;

    // Hieu ung am thanh chi tiet tung bo phan
    Mix_Chuck* select;
    Mix_Chuck* win;
    Mix_chuck* lose;

    bool isMenuPlaying = false;
    bool isGamePlaying = false;

public:
    Music();
    void init();

    void playMenu();
    void stopMenu();

    void playSelect();

    void playRes(bool);

    Music();
};
#endif // MUSIC_H
