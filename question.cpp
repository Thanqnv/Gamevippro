#include "question.h"

void qrender(SDL_Renderer *renderer)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, "res/gfx/question1.png");

    SDL_Rect dest;
    dest.x = (cnst::WIN_W - cnst::QUESTION_W) / 2;
    dest.y = (cnst::WIN_H - cnst::QUESTION_H) / 2;
    dest.w = cnst::QUESTION_W;
    dest.h = cnst::QUESTION_H;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_RenderPresent(renderer);
}
