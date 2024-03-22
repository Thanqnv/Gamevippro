#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 800;
const int NUM_IMAGES = 24;
const int IMAGE_SIZE = 30;

// Tên của các file ảnh
std::string imageFiles[NUM_IMAGES] = {
    "a.png", "b.png", "c.png", "d.png", "e.png", "f.png", "g.png", "h.png", "i.png", "k.png", "l.png", "m.png",
    "n.png", "o.png", "p.png", "q.png", "r.png", "s.png", "t.png", "u.png", "v.png", "w.png", "x.png", "y.png"};

// Đường dẫn của thư mục chứa các ảnh
std::string imageDir = "image/chu/";

int main(int argc, char* argv[])
{
    // Khởi tạo SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Khởi tạo SDL_image
    int imgFlags = IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    // Tải hình ảnh nền từ tệp JPEG
    SDL_Surface* backgroundSurface = IMG_Load("image/background.jpg");
    if (backgroundSurface == nullptr)
    {
        std::cerr << "Unable to load image! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo texture từ surface của hình ảnh nền
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface); // Giải phóng bộ nhớ sau khi tạo texture

    // Tải hình ảnh khác từ thư mục "image/cauhoi"
    SDL_Surface* cauhoiSurface = IMG_Load("image/cauhoi/cau1.jpg");
    if (cauhoiSurface == nullptr)
    {
        std::cerr << "Unable to load image from image/cauhoi! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Tạo texture từ surface của hình ảnh khác
    SDL_Texture* cauhoiTexture = SDL_CreateTextureFromSurface(renderer, cauhoiSurface);
    SDL_FreeSurface(cauhoiSurface); // Giải phóng bộ nhớ sau khi tạo texture

    // Tải hình ảnh khác từ thư mục "image/cauhoi"
    SDL_Surface* deSurface = IMG_Load("image/de.png");
    if (deSurface == nullptr)
    {
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

    // Load và hiển thị ảnh
    SDL_Texture* textures[NUM_IMAGES];
    SDL_Rect destRect[NUM_IMAGES]; // Mảng các SDL_Rect cho từng ảnh
    int x = 125, y = 560;

    for (int i = 0; i < NUM_IMAGES; ++i)
    {
        std::string imagePath = imageDir + imageFiles[i];
        SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
        if (imageSurface == nullptr)
        {
            std::cerr << "Failed to load image: " << imagePath << std::endl;
            continue;
        }


        // Thay đổi kích thước ảnh
        SDL_Surface* resizedSurface = SDL_CreateRGBSurface(0, IMAGE_SIZE, IMAGE_SIZE, 32, 0, 0, 0, 0);
        SDL_Rect stretchRect = {0, 0, IMAGE_SIZE, IMAGE_SIZE};
        SDL_BlitScaled(imageSurface, nullptr, resizedSurface, &stretchRect);

        textures[i] = SDL_CreateTextureFromSurface(renderer, resizedSurface);
        SDL_FreeSurface(imageSurface);
        SDL_FreeSurface(resizedSurface);

        if (textures[i] != nullptr)
        {
            SDL_QueryTexture(textures[i], nullptr, nullptr, &destRect[i].w, &destRect[i].h);
            destRect[i].x = x;
            destRect[i].y = y;
            SDL_RenderCopy(renderer, textures[i], nullptr, &destRect[i]);

            // Di chuyển đến vị trí của ảnh tiếp theo
            x += IMAGE_SIZE + 10;
            if (x >= 600 - IMAGE_SIZE)
            {
                x = 125;
                y += IMAGE_SIZE + 10;
            }
        }
    }

    // Hiển thị renderer lên cửa sổ
    SDL_RenderPresent(renderer);

    // Theo dõi trạng thái kéo thả
    bool isDragging = false;
    int draggedIndex = -1;
    int offsetX, offsetY;

    // Theo dõi sự kiện
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // Kiểm tra xem chuột có nằm trong một hình ảnh không
                    for (int i = 0; i < NUM_IMAGES; ++i)
                    {
                        if (event.button.x >= destRect[i].x && event.button.x <= destRect[i].x + destRect[i].w &&
                            event.button.y >= destRect[i].y && event.button.y <= destRect[i].y + destRect[i].h)
                        {
                            isDragging = true;
                            draggedIndex = i;
                            offsetX = event.button.x - destRect[i].x;
                            offsetY = event.button.y - destRect[i].y;
                            break;
                        }
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = false;
                    draggedIndex = -1;
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                if (isDragging && draggedIndex != -1)
                {
                    // Cập nhật vị trí mới của ảnh khi di chuyển
                    destRect[draggedIndex].x = event.motion.x - offsetX;
                    destRect[draggedIndex].y = event.motion.y - offsetY;
                }
            }
        }

        // Xóa màn hình
        SDL_RenderClear(renderer);

        // Vẽ hình ảnh nền lên renderer
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);

        // Vẽ hình ảnh khác lên renderer
        SDL_RenderCopy(renderer, cauhoiTexture, nullptr, &cauhoiRect);

        // Vẽ hình ảnh khác lên renderer
        SDL_RenderCopy(renderer, deTexture, nullptr, &deRect);

        // Hiển thị các ảnh đã di chuyển
        for (int i = 0; i < NUM_IMAGES; ++i)
        {
            SDL_RenderCopy(renderer, textures[i], nullptr, &destRect[i]);
        }

        // Hiển thị renderer lên cửa sổ
        SDL_RenderPresent(renderer);
    }

    // Giải phóng bộ nhớ và thoát
    for (int i = 0; i < NUM_IMAGES; ++i)
    {
        if (textures[i] != nullptr)
        {
            SDL_DestroyTexture(textures[i]);
        }
    }

    // Dọn dẹp
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(cauhoiTexture);
    SDL_DestroyTexture(deTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
