/// @file main.cpp
/// Contains the entrypoint for the program

#include <FEHLCD.h>
#include <FEHUtility.h>

#include <cstdint>
#include <iostream>

#include "game.h"
#include "menu.h"
#include "ui.h"
#include "util.h"

/// Main function which is the entrypoint for the entire program
int main() {
    current_scene = menu;

    while (true) {
        float t_start = TimeNow();
        touchPressed = LCD.Touch(&touchX, &touchY);
        LCD.Clear();

        current_scene->update(TimeNow() - t_start);
    }

    return 0;
}
