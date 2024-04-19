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
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0 ) std::cout << "SDL failed: " << SDL_GetError() << std::endl;          //SDL
    if(!IMG_Init(IMG_INIT_PNG)) std::cout << "IMG failed: " << IMG_GetError() << std::endl;                                 //images
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) std::cout << "Sound failed: " << Mix_GetError() << std::endl; //sounds

    // Lưu thời điểm ban đầu
    Uint32 startTime = SDL_GetTicks();

    // Khởi tạo SDL_ttf
    if(TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Tải font
    TTF_Font* font = TTF_OpenFont("BigShouldersText-Black.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    //create window and renderer
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cnst::WIN_W, cnst::WIN_H, SDL_WINDOW_SHOWN);
    if (window==NULL) std::cout << "Window failed: " << SDL_GetError() << std::endl;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //get monitor refresh rate
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(SDL_GetWindowDisplayIndex(window), 0, &mode);
    const float refreshRate = mode.refresh_rate;

    Media media(renderer);    //media manager/storage

    // Hiển thị cửa sổ
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

    Mix_PlayMusic(media.bgMusic, -1); //play music

    SDL_Event event;
    int startTicks;                         //time at start of frame
    int prevStartTicks = SDL_GetTicks();    //time since previous start of frame
    int prevFrameTicks;                     //duration of last frame
    int accumulator = 0;                    //time accumulated for decoupling frame rate and game loop

    // Load texture từ file ảnh
    SDL_Texture* questionTex1 = TextureManager::LoadTexture("res/gfx/question1.png", renderer);
    Chest chestObject(3 * cnst::TILE_SIZE, 40 * cnst::TILE_SIZE, media.chest1Tex, &media.chestClips, media.chestSfx, &player);
    Chest *chest = &chestObject; // Gán con trỏ chest bằng địa chỉ của chestObject

    bool tmp = false;
    bool gameRunning = true;
    while(gameRunning)
    {

        startTicks = SDL_GetTicks();
        prevFrameTicks = startTicks - prevStartTicks;
        prevStartTicks = startTicks;
        accumulator += prevFrameTicks;          //frame time is added to the accumulator
        while(accumulator >= cnst::TIME_STEP)   //if there is enough time accumulated to advance the simulation:
        {
            while(SDL_PollEvent(&event))        //take each event from the queue
            {
                if(event.type == SDL_QUIT) gameRunning = false;

                //process pressed keys (registered once)
                if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        if (player.canDoubleJump && !player.onGround) player.jump();
                        break;
                    case SDLK_LSHIFT:
                        player.dash();
                    }
                }
            }

            //process held keys (registered continuously)
            const Uint8 *keyState = SDL_GetKeyboardState(NULL);         //current state of keyboard

            if(keyState[SDL_SCANCODE_LEFT])                             //if left is being held
            {
                player.moveLeft();
            }
            if(keyState[SDL_SCANCODE_RIGHT])                            //if right is being held
            {
                player.moveRight();
            }
            if(keyState[SDL_SCANCODE_SPACE] && player.onGround)         //if space is being held and player is on ground
            {
                player.jump();
            }

            player.drag(0.9);                                                   //set default drag force
            if(!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])   //if not moving left or right
            {
                if (player.onGround) player.drag(0.7);                          //extra drag when standing on floor
                if (player.getAnim() != 0 && player.getAnim() != 3) player.setAnim(0, cnst::ANIM_REPEAT, 80);   //idle animation when not moving
            }

            //update entity physics

            for (auto i = entities.end() - 1; i >= entities.begin(); i--)   //iterate through entities (in reverse, so that deleting an entity will not skip the next)
            {
                Entity *e = *i;
                if (e->deleted)
                {
                    entities.erase(i);  //remove from entitiy list
                    continue;
                }
                e->updateGravity(0.5);
                e->updateCollisions(&map);
                e->updateKeyDoor(&map);
                e->updateChest(&map);
                e->updatePos();

            }


            if (chest->checkChecst(&map) && !tmp)
            {
                // Vẽ texture lên renderer
                TextureManager::DrawTexture(questionTex1, renderer, 250, 100, 500, 600);

                // Update renderer
                SDL_RenderPresent(renderer);

                tmp = true;


                bool congratulation = false;
                int guess = 5;

                while (!congratulation && guess > 0)
                {
                    // Tạo texture từ văn bản
                    SDL_Color textColor = {0, 0, 0};
                    int fontSize = 24; // Kích thước chữ

                    std::string answer = "university";
                    // Nhập xâu từ người dùng
                    std::string s;
                    std::cout << "Nhập xâu: ";
                    std::getline(std::cin, s);
                    // Tạo texture từ xâu vừa nhập
                    SDL_Texture* textTexture = TextureManager::RenderText(s, "BigShouldersText-Black.ttf", textColor, fontSize, renderer);

                    // Kiểm tra nếu không thể tạo texture từ văn bản
                    if (!textTexture)
                    {
                        std::cerr << "Failed to render text to texture!" << std::endl;
                        // Xử lý lỗi tại đây nếu cần thiết
                    } else
                    {
                        // Vẽ texture chữ lên renderer
                        TextureManager::DrawTexture(textTexture, renderer, 350, 400, 120, 100);

                        // Update renderer
                        SDL_RenderPresent(renderer);

                        // Chờ 1 giây
                        SDL_Delay(1000);

                        if (answer == s)
                        {
                            textColor = {0, 128, 0};
                            textTexture = TextureManager::RenderText(s, "BigShouldersText-Black.ttf", textColor, fontSize, renderer);
                            TextureManager::DrawTexture(textTexture, renderer, 350, 400, 120, 100);
                            // Update renderer
                            SDL_RenderPresent(renderer);
                            Mix_PlayChannel(-1, media.correctSfx, 0);
                            SDL_Delay(200);
                            congratulation = true;
                        }
                        else
                        {
                            textColor = {255, 0, 0};
                            textTexture = TextureManager::RenderText(s, "BigShouldersText-Black.ttf", textColor, fontSize, renderer);
                            TextureManager::DrawTexture(textTexture, renderer, 350, 400, 120, 100);
                            // Update renderer
                            SDL_RenderPresent(renderer);
                            Mix_PlayChannel(-1, media.wrongSfx, 0);
                            SDL_Delay(200);

                        }

                    }

                    // Giải phóng texture chữ
                    SDL_DestroyTexture(textTexture);
                    guess--;
                    TextureManager::DrawTexture(media.blockTex, renderer, 350, 400, 120, 100);
                    SDL_RenderPresent(renderer);
                }
                SDL_DestroyTexture(questionTex1);
                SDL_DestroyTexture(media.blockTex);
            }

            map.updateCamera(&player);          //update camera offsets and zones

            accumulator -= cnst::TIME_STEP;     //remove simulated time from the accumulator
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
        SDL_RenderClear(renderer);              //clear screen with dark blue

        //render entities
        map.drawParralax(renderer);
        map.drawMap(renderer);
        for (Entity *e : entities)
        {
            e->updateClips();
            e->render(-offsetX, -offsetY, renderer);
        }

        SDL_RenderPresent(renderer);     //present rendered image to screen

        //delay loop to match refresh rate
        int frameTicks = SDL_GetTicks() - startTicks;   //ticks since start of frame
        if(frameTicks < 1000/refreshRate)               //if frame was too quick
        {
            SDL_Delay(1000/refreshRate - frameTicks);   //delay
        }

    }

    SDL_DestroyWindow(window);   //destroy window
    SDL_Quit();                 //close SDL

    return 0;
}
