/// @file game.cpp
/// Main gameplay implementation

#include <algorithm>
#include <memory>

#include "FEHLCD.h"
#include "FEHUtility.h"

#include "game.h"
#include "menu.h"
#include "throwable.h"
#include "ui.h"
#include "util.h"

Game::Game() {
    exit_button = std::make_unique<UIButton>("X", UIPosition(10, 10));
    exit_button->bind_on_button_up([]() { current_scene = menu; });
}

void Game::start(float bomb_probability, float multiplier) {
    this->bomb_probability = bomb_probability;
    this->multiplier = multiplier;
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
void remove_foreach(T& a) {
    a.erase(std::remove_if(a.begin(), a.end(),
                           [](auto& b) { return b->get_should_be_removed(); }),
            a.end());
}

void Game::physics_update(double t, double dt) {
    physics_update_foreach(t, dt, apples);
    physics_update_foreach(t, dt, bananas);
    physics_update_foreach(t, dt, fruit_shards);
}

void Game::update(double alpha) {
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
    }

    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

    remove_foreach(apples);
    remove_foreach(bananas);
    remove_foreach(fruit_shards);

    update_foreach(alpha, apples);
    update_foreach(alpha, bananas);

    knife.update();

    update_foreach(alpha, fruit_shards);

    exit_button->update();
}