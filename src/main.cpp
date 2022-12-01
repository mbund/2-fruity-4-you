/// @file main.cpp
/// Contains the entrypoint for the program

#include <FEHLCD.h>
#include <FEHUtility.h>

#include <algorithm>
#include <cstdint>
#include <iostream>

#include "game.h"
#include "menu.h"
#include "ui.h"
#include "util.h"

/// Main function which is the entrypoint for the entire program
int main() {
    current_scene = menu;

    // https://gafferongames.com/post/fix_your_timestep
    double t = 0.0;
    double dt = 0.01;

    double current_time = TimeNow();
    double accumulator = 0.0;

    while (true) {
        double new_time = TimeNow();
        double frame_time = new_time - current_time;
        if (frame_time > 0.25)
            frame_time = 0.25;
        current_time = new_time;

        accumulator += frame_time;

        while (accumulator >= dt) {
            current_scene->physics_update(t, dt);
            t += dt;
            accumulator -= dt;
        }

        double alpha = accumulator / dt;

        touchPressed = LCD.Touch(&touchX, &touchY);
        touchX = std::clamp(touchX, 0, (int)LCD_WIDTH);
        touchY = std::clamp(touchY, 0, (int)LCD_HEIGHT);
        LCD.Clear();
        current_scene->update(alpha);
    }

    return 0;
}
