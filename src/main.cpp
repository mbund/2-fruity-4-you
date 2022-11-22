/// @file main.cpp
/// Contains the entrypoint for the program

#include <FEHLCD.h>

#include <cstdint>
#include <iostream>

#include "FEHUtility.h"
#include "game.h"
#include "main.h"
#include "menu.h"
#include "ui.h"

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

void Scene::update(float dt) {}