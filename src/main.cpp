/// @file main.cpp
/// Contains the entrypoint for the program

#include <FEHLCD.h>
#include <iostream>
#include "ui.h"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

/// Main function which is the entrypoint for the entire program
int main() {
    UIButton button1("click me 1", 20, 20);
    button1.bind_on_button_up(
        []() { std::cout << "button up 1" << std::endl; });
    button1.bind_on_button_down(
        []() { std::cout << "button down 1" << std::endl; });

    UIButton button2("click me 2", 20, 80);
    button2.bind_on_button_up(
        []() { std::cout << "button up 2" << std::endl; });
    button2.bind_on_button_down(
        []() { std::cout << "button down 2" << std::endl; });

    while (1) {
        LCD.Update();
        LCD.Clear();

        // border for debugging
        LCD.DrawRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

        button1.update();
        button2.update();
    }

    return 0;
}