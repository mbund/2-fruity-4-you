#include "FEHLCD.h"

#include "game.h"
#include "main.h"
#include "ui.h"

Game::Game() {}

void Game::start(float bomb_probability, float multiplier) {
    this->bomb_probability = bomb_probability;
    this->multiplier = multiplier;
}

void Game::update(float dt) {
    LCD.DrawRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

    LCD.DrawCircle(x, 50, 10);
    x += dt * this->multiplier * 50;

    if (touchPressed)
        LCD.DrawCircle(touchX, touchY, 2);
}