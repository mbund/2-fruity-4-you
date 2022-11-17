/// @file main.cpp
/// Contains the entrypoint for the program

#include "FEHLCD.h"

/// Main function which is the entrypoint for the entire program
int main() {
    LCD.WriteLine("Hello World!");

    while (1) {
        LCD.Update();
        // Never end
    }
    return 0;
}