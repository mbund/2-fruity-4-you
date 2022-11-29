/// @file game.cpp
/// Main gameplay implementation

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "FEHLCD.h"
#include "FEHUtility.h"

#include "endgame.h"
#include "game.h"
#include "menu.h"
#include "throwable.h"
#include "ui.h"
#include "util.h"

Game::Game() {
    exit_button = std::make_unique<UIButton>(
        "X", UIPosition(10, 10, UIPosition::TopRight));
    exit_button->bind_on_button_up([]() { current_scene = menu; });
}

void Game::start(float bomb_probability, float multiplier) {
    apples.clear();
    bananas.clear();
    bombs.clear();
    fruit_shards.clear();
    this->bomb_probability = bomb_probability;
    this->multiplier = multiplier;
    points = 0;
    combo = 0;
    t = 0;
    paused = false;
    time_started = TimeNow();
}

template <typename T>
void physics_update_foreach(double t, double dt, T& a) {
    std::for_each(a.begin(), a.end(),
                  [t, dt](auto& b) { b->physics_update(t, dt); });
}

template <typename T>
void update_foreach(double alpha, T& a) {
    std::for_each(a.begin(), a.end(), [alpha](auto& b) { b->update(alpha); });
}

template <typename T>
void remove_if_foreach(T& a) {
    a.erase(std::remove_if(a.begin(), a.end(),
                           [](auto& b) { return b->get_should_be_removed(); }),
            a.end());
}

void Game::physics_update(double t, double dt) {
    if (paused)
        return;

    physics_update_foreach(t, dt, apples);
    physics_update_foreach(t, dt, bananas);
    physics_update_foreach(t, dt, bombs);
    physics_update_foreach(t, dt, fruit_shards);

    this->t += dt;
}

void Game::update(double alpha) {
    auto num = std::to_string(points);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt(num.c_str(), 10, LCD_HEIGHT - FONT_GLYPH_HEIGHT - 10);

    if (rand_range(0.0f, 1.0f) < 0.025f) {
        Vector2 pos = {(float)LCD_WIDTH / 2, LCD_HEIGHT + 20};
        auto apple = std::make_unique<Apple>(pos, 8);
        apple->add_force(
            {rand_range(-80000, 80000), rand_range(-360000, -260000)});
        apples.push_back(std::move(apple));

        auto banana = std::make_unique<Bananas>(pos, 8);
        banana->add_force(
            {rand_range(-80000, 80000), rand_range(-360000, -260000)});
        bananas.push_back(std::move(banana));

        auto bomb = std::make_unique<Bomb>(pos, 8);
        bomb->add_force(
            {rand_range(-80000, 80000), rand_range(-360000, -260000)});
        bombs.push_back(std::move(bomb));
    }

    remove_if_foreach(apples);
    remove_if_foreach(bananas);
    remove_if_foreach(bombs);
    remove_if_foreach(fruit_shards);

    update_foreach(alpha, apples);
    update_foreach(alpha, bananas);
    update_foreach(alpha, bombs);

    if (!paused)
        knife.update();

    update_foreach(alpha, fruit_shards);

    if (paused) {
        double percent = std::min((TimeNow() - time_paused) / 2.0, 1.0);
        uint64_t max = LCD_WIDTH * LCD_HEIGHT;
        uint64_t travel = max * percent;
        for (size_t i = 0; i < travel; i++) {
            LCD.SetFontColor(BLACK);
            LCD.DrawPixel(i % LCD_WIDTH, i / LCD_WIDTH);
        }

        if (percent >= 1.0) {
            end_game->end(points);
            current_scene = end_game;
        }
    }
}