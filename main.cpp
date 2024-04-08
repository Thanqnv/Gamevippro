#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int PLAYER_SPEED = 15; // Tăng tốc độ di chuyển

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* player = NULL;

// Animation clip structure
struct Clip {
    int x, y;
    int w, h;
};

// Function to load texture from file
SDL_Texture* loadTexture(const std::string& file) {
    SDL_Surface* surface = IMG_Load(file.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << SDL_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Function to render texture at specified position
void renderTexture(SDL_Texture* texture, int x, int y, SDL_Rect* clip = NULL) {
    SDL_Rect rect = { x, y, 0, 0 };
    if (clip) {
        rect.w = clip->w;
        rect.h = clip->h;
    } else {
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    }
    SDL_RenderCopy(renderer, texture, clip, &rect);
}

// Function to create animation clips
std::vector<Clip> makeClips(int startX, int numFrames, int width, int height) {
    std::vector<Clip> clips;
    for (int i = 0; i < numFrames; ++i) {
        Clip clip = { startX + i * width, 0, width, height };
        clips.push_back(clip);
    }
    return clips;
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("Control Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load player texture
    std::string basePath = "res/gfx/";
    player = loadTexture(basePath + "player.png");
    if (!player) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Animation clips
    std::vector<std::vector<Clip>> playerClips;
    playerClips.push_back(makeClips(0, 4, 70, 70)); // First animation set
    playerClips.push_back(makeClips(1, 4, 70, 70)); // Second animation set
    playerClips.push_back(makeClips(2, 4, 70, 70)); // Third animation set
    playerClips.push_back(makeClips(3, 4, 70, 70)); // Fourth animation set

    // Initial position of player
    int playerX = (WIN_WIDTH - 64) / 2;
    int playerY = (WIN_HEIGHT - 64) / 2;

    // Main loop
    bool quit = false;
    int animationIndex = 0;
    int frameIndex = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        playerY -= PLAYER_SPEED;
                        if (playerY < 0) playerY = 0; // Giới hạn không vượt quá biên trên
                        break;
                    case SDLK_DOWN:
                        playerY += PLAYER_SPEED;
                        if (playerY + playerClips[animationIndex][frameIndex].h > WIN_HEIGHT) // Giới hạn không vượt quá biên dưới
                            playerY = WIN_HEIGHT - playerClips[animationIndex][frameIndex].h;
                        break;
                    case SDLK_LEFT:
                        playerX -= PLAYER_SPEED;
                        if (playerX < 0) playerX = 0; // Giới hạn không vượt quá biên trái
                        break;
                    case SDLK_RIGHT:
                        playerX += PLAYER_SPEED;
                        if (playerX + playerClips[animationIndex][frameIndex].w > WIN_WIDTH) // Giới hạn không vượt quá biên phải
                            playerX = WIN_WIDTH - playerClips[animationIndex][frameIndex].w;
                        break;
                    default:
                        break;
                }
            }
        }

        // Clear renderer
        SDL_RenderClear(renderer);

        // Get current clip rectangle
        SDL_Rect clipRect = { playerClips[animationIndex][frameIndex].x, playerClips[animationIndex][frameIndex].y,
                               playerClips[animationIndex][frameIndex].w, playerClips[animationIndex][frameIndex].h };

        // Render player animation at current position
        renderTexture(player, playerX, playerY, &clipRect);

        // Present renderer
        SDL_RenderPresent(renderer);

        // Delay for a short period to control animation speed
        SDL_Delay(100); // Adjust this value to change animation speed

        // Update frame index for animation
        frameIndex = (frameIndex + 1) % playerClips[animationIndex].size();
    }

    // Cleanup
    SDL_DestroyTexture(player);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
