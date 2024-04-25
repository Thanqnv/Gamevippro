#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "cnst.h"
#include "media.h"
#include "entity.h"
#include "map.h"
#include "question.h"


int main(int argc, char* args[])
{
    //init systems
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0 ) std::cout << "SDL failed: " << SDL_GetError() << std::endl;
    if(!IMG_Init(IMG_INIT_PNG)) std::cout << "IMG failed: " << IMG_GetError() << std::endl;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) std::cout << "Sound failed: " << Mix_GetError() << std::endl;

    Uint32 startTime = SDL_GetTicks();

    if(TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("BigShouldersText-Black.ttf", 20);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cnst::WIN_W, cnst::WIN_H, SDL_WINDOW_SHOWN);
    if (window==NULL) std::cout << "Window failed: " << SDL_GetError() << std::endl;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_DisplayMode mode;
    SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(window), 0, &mode);
    const float refreshRate = mode.refresh_rate;

    Media media(renderer);

    SDL_RenderPresent(renderer);

    int offsetX, offsetY, targetOffsetX, targetOffsetY;

    Map map(&media, &offsetX, &offsetY, &targetOffsetX, &targetOffsetY);

    //player
    AnimEntity dust1(-100, -100, media.dustTex, cnst::TILE_SIZE, cnst::TILE_SIZE, &media.dustClips);
    AnimEntity dust2(-100, -100, media.dustTex, cnst::TILE_SIZE, cnst::TILE_SIZE, &media.dustClips);
    Player player(3 * cnst::TILE_SIZE, 31 * cnst::TILE_SIZE, media.playerTex, &media.playerClips, media.jumpSfx, media.dashSfx, media.thudSfx, &dust1, &dust2);
    player.setAnim(0, cnst::ANIM_REPEAT, 80);

    //doors
    Door door1(16 * cnst::TILE_SIZE, 28 * cnst::TILE_SIZE, media.doorTex, &media.doorClips, media.doorSfx, &player);
    Door door2(23 * cnst::TILE_SIZE, 19 * cnst::TILE_SIZE, media.doorTex, &media.doorClips, media.doorSfx, &player);

    //keys
    Key key1(30 * cnst::TILE_SIZE, 43 * cnst::TILE_SIZE, media.keyTex, &media.keyClips, media.keySfx, &player);
    Key key2(13 * cnst::TILE_SIZE, 11 * cnst::TILE_SIZE, media.keyTex, &media.keyClips, media.keySfx, &player);

    //tutorial
    Entity tutorial(6 * cnst::TILE_SIZE, 26 * cnst::TILE_SIZE, media.tutorialTex, 450, 250, 0);

    //chest
    Chest chest1(3 * cnst::TILE_SIZE, 40 * cnst::TILE_SIZE, media.chest1Tex, &media.chestClips, media.chestSfx, &player);
    Chest chest2(23 * cnst::TILE_SIZE, 30 * cnst::TILE_SIZE, media.chest2Tex, &media.chestClips, media.chestSfx, &player);
    Chest chest3(27 * cnst::TILE_SIZE, 3 * cnst::TILE_SIZE, media.chest3Tex, &media.chestClips, media.chestSfx, &player);
    Chest chest4(27 * cnst::TILE_SIZE, 21 * cnst::TILE_SIZE, media.chest4Tex, &media.chestClips, media.chestSfx, &player);

    //quai
    Quai quai(25 * cnst::TILE_SIZE, 3 * cnst::TILE_SIZE, media.quaiTex, &media.quaiClips, &player);


    //entities in the game to be iteratively updated    (in render order)
    std::vector<Entity *> entities;
    entities.push_back(&tutorial);
    entities.push_back(&door1);
    entities.push_back(&door2);
    entities.push_back(&key1);
    entities.push_back(&key2);
    entities.push_back(&dust1);
    entities.push_back(&dust2);
    entities.push_back(&player);
    entities.push_back(&chest1);
    entities.push_back(&chest2);
    entities.push_back(&chest3);
    entities.push_back(&chest4);
    entities.push_back(&quai);

    SDL_Event event;
    int startTicks;
    int prevStartTicks = SDL_GetTicks();
    int prevFrameTicks;
    int accumulator = 0;

    Mix_PlayMusic(media.bgMenu, -1);
    bool play = false;
    while (!play)
    {

        TextureManager::DrawTexture(media.menuTex, renderer, 0, 0, cnst::WIN_W, cnst::WIN_H);

        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                play = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mX = event.button.x;
                int mY = event.button.y;
                if (mX >= 370 && mX <= 660 && mY >= 650 && mY <= 750)
                {
                    std::cout << mX << " " << mY;
                    SDL_DestroyTexture(media.menuTex);
                    Mix_FreeMusic(media.bgMenu);
                    media.bgMenu = nullptr;
                    play = true;
                }
            }
        }
    }

    Mix_PlayMusic(media.bgMusic, -1);
    SDL_Rect dest;

    dest.x = 250;
    dest.y = 100;
    dest.w = 500;
    dest.h = 600;

    SDL_Texture* tempTexture = nullptr;

    int k = 1, tmp = 0;
    std::string s = "";
    Uint32 x;
    SDL_Color textColor = {0, 0, 0};

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, s.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect rect = {300, 500, (s.length() * 20) - 6 - (2 * s.length()), 50};
    int guess = 3;
    std::string answer = "16761";
    bool win = false;
    bool gameRunning = true;
    while(gameRunning)
    {
        startTicks = SDL_GetTicks();
        prevFrameTicks = startTicks - prevStartTicks;
        prevStartTicks = startTicks;
        accumulator += prevFrameTicks;
        while(accumulator >= cnst::TIME_STEP)
        {
            if (SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) gameRunning = false;


                if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        if (player.canDoubleJump && !player.onGround) player.jump();
                        break;
                    case SDLK_LSHIFT:
                        player.dash();
                        break;
                    }
                }
            }

            const Uint8 *keyState = SDL_GetKeyboardState(NULL);

            if(keyState[SDL_SCANCODE_LEFT])
            {
                player.moveLeft();
            }
            if(keyState[SDL_SCANCODE_RIGHT])
            {
                player.moveRight();
            }
            if(keyState[SDL_SCANCODE_SPACE] && player.onGround)
            {
                player.jump();
            }

            player.drag(0.9);
            if(!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
            {
                if (player.onGround) player.drag(0.7);
                if (player.getAnim() != 0 && player.getAnim() != 3) player.setAnim(0, cnst::ANIM_REPEAT, 80);
            }


            for (auto i = entities.end() - 1; i >= entities.begin(); i--)
            {
                Entity *e = *i;
                if (e->deleted)
                {
                    entities.erase(i);
                    continue;
                }
                e->updateGravity(0.5);
                e->updateCollisions(&map);
                e->updateKeyDoor(&map);
                e->updateChest(&map);
                e->updatePos();
            }


            if (chest1.checkChest(&map))
            {
                Chest sao(3 * cnst::TILE_SIZE, 38 * cnst::TILE_SIZE, media.saoTex, &media.chestClips, media.chestSfx, &player);
                entities.push_back(&sao);
            }

            if (chest2.checkChest(&map))
            {
                Chest sao2(23 * cnst::TILE_SIZE, 28 * cnst::TILE_SIZE, media.sao2Tex, &media.chestClips, media.chestSfx, &player);
                entities.push_back(&sao2);
            }

            if (chest3.checkChest(&map))
            {
                Chest sao2(27 * cnst::TILE_SIZE, 1 * cnst::TILE_SIZE, media.sao3Tex, &media.chestClips, media.chestSfx, &player);
                entities.push_back(&sao2);
            }

            if (chest4.checkChest(&map) && guess >= 0)
            {

                tempTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 0, 0);
                SDL_SetRenderTarget(renderer, tempTexture);
                SDL_RenderCopy(renderer, media.questionTex, NULL, &dest);
                SDL_SetRenderTarget(renderer, NULL);

                SDL_RenderCopy(renderer, tempTexture, NULL, &dest);
                SDL_RenderPresent(renderer);

                int length = 1;
                if (guess > 0)
                {
                    if (s.length() == 5)
                    {
                        s = "";
                    }
                    while (s.length() < 5)
                    {
                    if (SDL_PollEvent(&event))
                    {
                        if(event.type == SDL_QUIT) gameRunning = false;

                        if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
                        {

                            switch(event.key.keysym.sym)
                            {
                            case SDLK_1:
                                s = s + "1";
                                std::cout << s << " ";
                                break;
                            case SDLK_2:
                                s = s + "2";
                                std::cout << s << " ";
                                break;
                            case SDLK_3:
                                s = s + "3";
                                std::cout << s << " ";
                                break;
                            case SDLK_4:
                                s = s + "4";
                                std::cout << s << " ";
                                break;
                            case SDLK_5:
                                s = s + "5";
                                std::cout << s << " ";
                                break;
                            case SDLK_6:
                                s = s + "6";
                                std::cout << s << " ";
                                break;
                            case SDLK_7:
                                s = s + "7";
                                std::cout << s << " ";
                                break;
                            case SDLK_8:
                                s = s + "8";
                                std::cout << s << " ";
                                break;
                            case SDLK_9:
                                s = s + "9";
                                std::cout << s << " ";
                                break;
                            case SDLK_0:
                                s = s + "0";
                                std::cout << s << " ";
                                break;
                            case SDLK_RETURN:
                                break;
                            }
                    }
                    }
                    if (s.length() < 5)
                    {

                        textColor = {0, 0, 0};
                        textSurface = TTF_RenderUTF8_Blended(font, s.c_str(), textColor);

                        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                        rect = {300, 500, (s.length() * 20) - 5 - (2 * s.length()), 50};

                        SDL_RenderCopy(renderer, textTexture, NULL, &rect);
                        SDL_RenderPresent(renderer);
                        length++;
                    }

                    }
                    if (s == answer && s.length() == 5)
                    {
                        textColor = {0, 128, 0};

                        textSurface = TTF_RenderUTF8_Blended(font, s.c_str(), textColor);
                        if (!textSurface) {
                            std::cout << "not load textSurface!";
                        }

                        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (!textTexture) {
                            std::cout << "not load textTexture!";
                        }

                        SDL_FreeSurface(textSurface);

                        rect = {300, 500, (s.length() * 20) - (2 * s.length()), 50};

                        SDL_RenderCopy(renderer, textTexture, NULL, &rect);
                        SDL_RenderPresent(renderer);
                        Mix_PlayChannel(-1, media.correctSfx, 0);
                        TextureManager::DrawTexture(media.youwinTex, renderer, 250, 100, 500, 600);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(700);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        gameRunning = false;
                        std::cout << gameRunning;
                    }
                    if (s != answer && s.length() == 5)
                    {
                        textColor = {255, 0, 0};

                        textSurface = TTF_RenderUTF8_Blended(font, s.c_str(), textColor);
                        if (!textSurface) {
                            std::cout << "not load textSurface!";
                        }

                        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                        if (!textTexture) {
                            std::cout << "not load textTexture!";
                        }

                        SDL_FreeSurface(textSurface);

                        rect = {300, 500, (s.length() * 20) - (2 * s.length()), 50};
                        SDL_RenderCopy(renderer, textTexture, NULL, &rect);
                        SDL_RenderPresent(renderer);
                        Mix_PlayChannel(-1, media.wrongSfx, 0);
                        SDL_Delay(300);
                        guess --;
                    }
                    if (guess == 0)
                    {
                        Uint32 x = SDL_GetTicks();
                        TextureManager::DrawTexture(media.gameoverTex, renderer, 250, 100, 500, 600);

                        SDL_RenderPresent(renderer);
                        SDL_Delay(700);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        gameRunning = false;
                        std::cout << gameRunning;
                    }
                }

            }

            if (k <= 200 && tmp == 0)
            {
                quai.setPos((25 * cnst::TILE_SIZE) + k, 3 * cnst::TILE_SIZE);
                k++;
                if (k == 200) tmp = 1;
            }
            else
            {
                quai.setPos((25 * cnst::TILE_SIZE) + k, 3 * cnst::TILE_SIZE);
                k--;
                if (k == 1) tmp = 0;
            }

            if (quai.checkQuai(&map))
            {
                Uint32 x = SDL_GetTicks();
                TextureManager::DrawTexture(media.gameoverTex, renderer, 250, 100, 500, 600);

                while (SDL_GetTicks() - x < 3000)
                {
                    SDL_RenderPresent(renderer);

                }
                SDL_DestroyTexture(media.gameoverTex);
                gameRunning = false;

            }
            map.updateCamera(&player);

            accumulator -= cnst::TIME_STEP;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
        SDL_RenderClear(renderer);

        map.drawParralax(renderer);
        map.drawMap(renderer);
        for (Entity *e : entities)
        {
            e->updateClips();
            e->render(-offsetX, -offsetY, renderer);
        }

        SDL_RenderPresent(renderer);

        int frameTicks = SDL_GetTicks() - startTicks;
        if(frameTicks < 1000/refreshRate)
        {
            SDL_Delay(1000/refreshRate - frameTicks);
        }

    }


    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
