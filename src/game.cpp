/// @file game.cpp
/// Main gameplay implementation

#include <algorithm>
#include <memory>

#include "FEHLCD.h"
#include "FEHUtility.h"

#include "game.h"
#include "throwable.h"
#include "ui.h"
#include "util.h"

Game::Game() {}

void Game::start(float bomb_probability, float multiplier) {
    this->bomb_probability = bomb_probability;
    this->multiplier = multiplier;
    time_started = TimeNow();
}

void Game::update(float dt) {
    if (rand_range(0.0f, 1.0f) < 0.01f) {
        auto a = Apple({(float)LCD_WIDTH / 2, LCD_HEIGHT + 20}, 8);
        a.add_force({rand_range(-4, 4), rand_range(-12, -8)});
        apples.push_back(a);
    }

    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

    std::for_each(apples.begin(), apples.end(),
                  [dt](Apple& a) { a.update(dt); });
    knife.update();
}