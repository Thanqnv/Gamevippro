#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 800;


int main(int argc, char* argv[]) {

    // Khởi tạo SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Khởi tạo SDL_image
    int imgFlags = IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    // Tải hình ảnh nền từ tệp JPEG
    SDL_Surface* backgroundSurface = IMG_Load("image/background.jpg");
    if (backgroundSurface == nullptr) {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo texture từ surface của hình ảnh nền
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface); // Giải phóng bộ nhớ sau khi tạo texture

    // Tải hình ảnh khác từ thư mục "image/cauhoi"
    SDL_Surface* cauhoiSurface = IMG_Load("image/cauhoi/cau1.jpg");
    if (cauhoiSurface == nullptr) {
        std::cerr << "Unable to load image from image/cauhoi! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo texture từ surface của hình ảnh khác
    SDL_Texture* cauhoiTexture = SDL_CreateTextureFromSurface(renderer, cauhoiSurface);
    SDL_FreeSurface(cauhoiSurface); // Giải phóng bộ nhớ sau khi tạo texture

    // Tải hình ảnh khác từ thư mục "image/cauhoi"
    SDL_Surface* deSurface = IMG_Load("image/de.png");
    if (deSurface == nullptr) {
        std::cerr << "Unable to load image from image! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo texture từ surface của hình ảnh khác
    SDL_Texture* deTexture = SDL_CreateTextureFromSurface(renderer, deSurface);
    SDL_FreeSurface(deSurface); // Giải phóng bộ nhớ sau khi tạo texture


    // Xác định vị trí và kích thước của hình ảnh nền
    SDL_Rect backgroundRect;
    backgroundRect.x = 0;
    backgroundRect.y = 0;
    backgroundRect.w = SCREEN_WIDTH;
    backgroundRect.h = SCREEN_HEIGHT;

    // Xác định vị trí và kích thước của hình ảnh khác
    SDL_Rect cauhoiRect;
    cauhoiRect.x = 110; // Vị trí x
    cauhoiRect.y = 100; // Vị trí y
    cauhoiRect.w = 500; // Chiều rộng
    cauhoiRect.h = 400; // Chiều cao

    // Xác định vị trí và kích thước của hình ảnh khác
    SDL_Rect deRect;
    deRect.x = 105; // Vị trí x
    deRect.y = 490; // Vị trí y
    deRect.w = 522; // Chiều rộng
    deRect.h = 200; // Chiều cao

    // Xóa màn hình
    SDL_RenderClear(renderer);

    // Vẽ hình ảnh nền lên renderer
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);

    // Vẽ hình ảnh khác lên renderer
    SDL_RenderCopy(renderer, cauhoiTexture, nullptr, &cauhoiRect);

    // Vẽ hình ảnh khác lên renderer
    SDL_RenderCopy(renderer, deTexture, nullptr, &deRect);


    for (int i = 0; i < 8; i++){

    }

    // Hiển thị renderer lên cửa sổ
    SDL_RenderPresent(renderer);

    // Chờ sự kiện kết thúc
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Dọn dẹp
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
