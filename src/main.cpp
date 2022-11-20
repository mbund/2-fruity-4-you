/// @file main.cpp
/// Contains the entrypoint for the program

#include <FEHLCD.h>
#include <cstdint>
#include <iostream>
#include "menu.h"

/// Main function which is the entrypoint for the entire program
int main() {
    Menu menu;
    
    while (1) {
        LCD.Update();
        LCD.Clear();

        // border for debugging
        // LCD.DrawRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

        menu.update();
    }

    return 0;
}