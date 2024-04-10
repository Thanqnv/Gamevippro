#pragma once

//hằng số được sử dụng trong suốt trò chơi(constants used throughout the game

namespace cnst
{
    constexpr int WIN_W = 1000;
    constexpr int WIN_H = 800;
    constexpr int TILE_SIZE = 50;
    constexpr int TIME_STEP = 10;

    //chế độ hoạt hình(animation modes)
    constexpr int ANIM_REPEAT = 0; // LOOP
    constexpr int ANIM_END = 1; // play once and stop
    constexpr int ANIM_DEFAULT = 2; // play once, set animNO to 0 and loop

    constexpr int TERMINAL_VEL = 23;
}
