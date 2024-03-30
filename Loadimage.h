#ifndef IMAGE_H
#define IMAGE_H

#include <string>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 800;
const int NUM_IMAGES = 24;
const int IMAGE_SIZE = 30;

extern std::string imageFiles[NUM_IMAGES];
extern std::string imageDir;

void runSDL();

#endif // IMAGE_H
