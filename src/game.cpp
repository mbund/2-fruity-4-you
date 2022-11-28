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

void Game::physics_update(double t, double dt) {
    std::for_each(apples.begin(), apples.end(),
                  [t, dt](auto& apple) { apple->physics_update(t, dt); });
    std::for_each(
        apple_shards.begin(), apple_shards.end(),
        [t, dt](auto& apple_shard) { apple_shard->physics_update(t, dt); });
}

void Game::update(double alpha) {
    if (rand_range(0.0f, 1.0f) < 0.025f) {
        Vector2 pos = {(float)LCD_WIDTH / 2, LCD_HEIGHT + 20};
        auto apple = std::make_unique<Apple>(pos, 8);
        apple->add_force(
            {rand_range(-80000, 80000), rand_range(-360000, -260000)});
        apples.push_back(std::move(apple));
    }

    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(0, 0, LCD_WIDTH, LCD_HEIGHT);

    apples.erase(std::remove_if(apples.begin(), apples.end(),
                                [alpha](auto& apple) {
                                    return apple->get_should_be_removed();
                                }),
                 apples.end());

    apple_shards.erase(
        std::remove_if(apple_shards.begin(), apple_shards.end(),
                       [alpha](auto& apple_shard) {
                           return apple_shard->get_should_be_removed();
                       }),
        apple_shards.end());

    std::for_each(apples.begin(), apples.end(),
                  [alpha](auto& apple) { apple->update(alpha); });

    knife.update();

    std::for_each(apple_shards.begin(), apple_shards.end(),
                  [alpha](auto& apple_shard) { apple_shard->update(alpha); });
}