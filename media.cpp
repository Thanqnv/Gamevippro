#include "media.h"

//struct for loading and storing graphics/sounds and generating animation clips
Media::Media(SDL_Renderer *renderer)
: renderer(renderer)
{

    //textures
    playerTex = loadTexture("res/gfx/player.png");
    dustTex = loadTexture("res/gfx/dust.png");
    doorTex = loadTexture("res/gfx/door.png");
    keyTex = loadTexture("res/gfx/key.png");

    starTex = loadTexture("res/gfx/starfield.png");
    brickFloorTex = loadTexture("res/gfx/floor.png");
    brickBackgroundTex = loadTexture("res/gfx/background.png");
    windowTex = loadTexture("res/gfx/window.png");
    tutorialTex = loadTexture("res/gfx/tutorial.png");

    chest1Tex = loadTexture("res/gfx/chest1.png");
    chest2Tex = loadTexture("res/gfx/chest2.png");
    chest3Tex = loadTexture("res/gfx/chest3.png");
    chest4Tex = loadTexture("res/gfx/chest4.png");
    questionTex = loadTexture("res/gfx/question1.png");
    blockTex = loadTexture("res/gfx/block.png");
    menuTex = loadTexture("res/gfx/menu.png");
    saoTex = loadTexture("res/gfx/sao.png");
    sao2Tex = loadTexture("res/gfx/sao2.png");
    sao3Tex = loadTexture("res/gfx/sao3.png");
    quaiTex = loadTexture("res/gfx/quai.png");
    gameoverTex = loadTexture("res/gfx/gameover.png");
    youwinTex = loadTexture("res/gfx/youwin.png");
    //animation clips
    playerClips = {
        makeClips(0, 4, 70, 70),
        makeClips(1, 4, 70, 70),
        makeClips(2, 4, 70, 70),
        makeClips(3, 4, 70, 70)};
    dustClips = {
        makeClips(0, 6, 50, 50),
        makeClips(1, 6, 50, 50),
        makeClips(2, 5, 50, 50),
        makeClips(3, 5, 50, 50),
        makeClips(4, 5, 50, 50),
        makeClips(5, 5, 50, 50)};
    doorClips = {
        makeClips(0, 1, 50, 200),
        makeClips(1, 15, 50, 200)};
    keyClips = {
        makeClips(0, 1, 50, 50),
        makeClips(1, 1, 50, 50)};
    chestClips = {
        makeClips(0, 1, 100, 100),
        makeClips(1, 1, 100, 100)};
    quaiClips = {
        makeClips(0, 1, 50, 50)};


    //sound
    bgMusic = loadMusic("res/wav/music.wav");
    bgMenu = loadMusic("res/wav/menu.wav");
    jumpSfx = loadSfx("res/wav/jump.wav");
    thudSfx = loadSfx("res/wav/thud.wav");
    dashSfx = loadSfx("res/wav/dash.wav");
    keySfx = loadSfx("res/wav/key.wav");
    doorSfx = loadSfx("res/wav/door.wav");
    chestSfx = loadSfx("res/wav/chest.wav");
    wrongSfx = loadSfx("res/wav/wrong.wav");
    correctSfx = loadSfx("res/wav/correct.wav");
}

    //load png, return texture pointer
SDL_Texture *Media::loadTexture(const char *filePath)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    if(texture==NULL) std::cout << "Texture failed: " << SDL_GetError() << std::endl;
    return texture;
}

Mix_Music *Media::loadMusic(const char *filePath)
{
    Mix_Music *music = NULL;
    music = Mix_LoadMUS(filePath);
    if(music==NULL) std::cout << "Music failed: " << Mix_GetError() << std::endl;
    return music;
}

Mix_Chunk *Media::loadSfx(const char *filePath)
{
    Mix_Chunk *sfx = NULL;
    sfx = Mix_LoadWAV(filePath);
    if(sfx==NULL) std::cout << "Sfx failed: " << Mix_GetError() << std::endl;
    return sfx;
}


//make a vector of points representing the top left corner of each animation frame
std::vector<SDL_Point> Media::makeClips(int animNo, int frames, int w, int h)
{
    std::vector<SDL_Point> clips;
    int y = animNo * h;
    for (int i = 0; i < frames; i++)
    {
        int x = i * w;
        SDL_Point p{x, y};
        clips.push_back(p);
    }
    return clips;
}


