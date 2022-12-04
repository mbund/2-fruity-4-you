/// @file game.cpp
/// @author John Ulm and Mark Bundschuh
/// @brief Main gameplay implementation

#include <math.h>
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

/// @author Mark Bundschuh
Game::Game() {
    background = image_repository->load_image("assets/background-menu.png");
}

/// @author Mark Bundschuh
void Game::start(float bomb_probability, float multiplier) {
    apples.clear();
    bananas.clear();
    bombs.clear();
    oranges.clear();
    cherries.clear();
    strawberries.clear();
    pineapples.clear();
    fruit_shards.clear();
    this->bomb_probability = bomb_probability;
    this->multiplier = multiplier;
    points = 0;
    combo = 0;
    time_started = TimeNow();
}

/// @author Mark Bundschuh
void Game::end() {
    // screen wipe
    LCD.SetFontColor(BLACK);
    for (uint64_t i = 0; i < LCD_HEIGHT; i += 2) {
        LCD.DrawHorizontalLine(i, 0, LCD_WIDTH);
        LCD.DrawHorizontalLine(i + 1, 0, LCD_WIDTH);
        LCD.Update();
    }

    // switch scenes and show keyboard/end game screen
    end_game->end(points);
    current_scene = end_game;
}

/// @author Mark Bundschuh
template <typename T>
void collision(Vector2 p1, Vector2 p2, T& a) {
    std::for_each(a.begin(), a.end(),
                  [p1, p2](auto& b) { b->collision(p1, p2); });
}

/// @author Mark Bundschuh
void Game::collide_with_knife(Vector2 p1, Vector2 p2) {
    collision(p1, p2, apples);
    collision(p1, p2, bananas);
    collision(p1, p2, oranges);
    collision(p1, p2, cherries);
    collision(p1, p2, strawberries);
    collision(p1, p2, pineapples);
    collision(p1, p2, bombs);
}

/// @author Mark Bundschuh
template <typename T>
void physics_update_foreach(double t, double dt, T& a) {
    std::for_each(a.begin(), a.end(),
                  [t, dt](auto& b) { b->physics_update(t, dt); });
}

/// @author Mark Bundschuh
template <typename T>
void update_foreach(double alpha, T& a) {
    std::for_each(a.begin(), a.end(), [alpha](auto& b) { b->update(alpha); });
}

/// @author Mark Bundschuh
template <typename T>
void remove_if_foreach(T& a) {
    a.erase(std::remove_if(a.begin(), a.end(),
                           [](auto& b) { return b->get_should_be_removed(); }),
            a.end());
}

/// @author John Ulm
void Game::physics_update(double t, double dt) {
    
    // random generation of fruits
    float randX = rand_range(20, LCD_WIDTH - 20);
    float randForce = rand_range(-50, 80000);

    if (randX > (float)LCD_WIDTH / 2)
        randForce *= -1.0;

    Vector2 pos = {randX, LCD_HEIGHT + 20};
    Vector2 first_force = {randForce, rand_range(-360000, -260000)};

    // There is a 1.5% chance every physics update (which keeps it in real
    // time) to spawn any item. There is a bomb_probability (difficulty level)
    // chance of spawning a bomb and if the item turns out to not be a bomb it
    // will uniformly randomly select from one of the five fruit to spawn.
    if (rand_range(0, 1) <= 0.015) {
        if (rand_range(0, 1) <= bomb_probability) {
            auto bomb = std::make_unique<Bomb>(pos);
            bomb->add_force(first_force);
            bombs.push_back(std::move(bomb));
        } else {
            uint32_t choice = (uint32_t)rand_range(0, 5 + 1);

            if (choice == 0) {
                auto apple = std::make_unique<Apple>(pos);
                apple->add_force(first_force);
                apples.push_back(std::move(apple));
            } else if (choice == 1) {
                auto banana = std::make_unique<Bananas>(pos);
                banana->add_force(first_force);
                bananas.push_back(std::move(banana));
            } else if (choice == 2) {
                auto orange = std::make_unique<Orange>(pos);
                orange->add_force(first_force);
                oranges.push_back(std::move(orange));
            } else if (choice == 3) {
                auto cherry = std::make_unique<Cherries>(pos);
                cherry->add_force(first_force);
                cherries.push_back(std::move(cherry));
            } else if (choice == 4) {
                auto strawberry = std::make_unique<Strawberry>(pos);
                strawberry->add_force(first_force);
                strawberries.push_back(std::move(strawberry));
            } else if (choice == 5) {
                auto pineapple = std::make_unique<Pineapple>(pos);
                pineapple->add_force(first_force);
                pineapples.push_back(std::move(pineapple));
            }
        }
    }

    physics_update_foreach(t, dt, apples);
    physics_update_foreach(t, dt, bananas);
    physics_update_foreach(t, dt, oranges);
    physics_update_foreach(t, dt, cherries);
    physics_update_foreach(t, dt, strawberries);
    physics_update_foreach(t, dt, pineapples);
    physics_update_foreach(t, dt, bombs);
    physics_update_foreach(t, dt, fruit_shards);
}

/// @author John Ulm
void Game::update(double alpha) {
    // render background
    background->render(LCD_WIDTH / 2, LCD_HEIGHT / 2, 0);

    // display score
    const uint32_t CORNER_OFFSET = 15;
    auto num = std::to_string(points);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt(num.c_str(), CORNER_OFFSET,
                LCD_HEIGHT - FONT_GLYPH_HEIGHT - CORNER_OFFSET);

    // display time
    auto time_left = (int)(GAME_DURATION + 1 - (TimeNow() - time_started));
    if (time_left >= 10) {
        LCD.WriteAt(time_left, CORNER_OFFSET, CORNER_OFFSET);
    } else {
        LCD.WriteAt(0, CORNER_OFFSET, CORNER_OFFSET);
        LCD.WriteAt(time_left, CORNER_OFFSET + FONT_GLYPH_WIDTH, CORNER_OFFSET);
    }

    // display combo and combo time
    const float COMBO_DUR = 2.0;
    if (TimeNow() - combo_time > COMBO_DUR) {
        combo = 0;
    }

    unsigned int color1 = 0xFF4545;
    unsigned int color2 = 0x214545 + game->combo * 0x050000;

    if (game->combo != 0) {
        LCD.SetFontColor(std::min(color1, color2));
        LCD.FillRectangle(
            LCD_WIDTH - (CORNER_OFFSET + 5 + 2 * FONT_GLYPH_WIDTH),
            CORNER_OFFSET - 5, 10 + 2 * FONT_GLYPH_WIDTH,
            10 + FONT_GLYPH_HEIGHT);

        LCD.SetFontColor(0xffaaaaaa);
        LCD.DrawRectangle(
            LCD_WIDTH - (CORNER_OFFSET + 5 + 2 * FONT_GLYPH_WIDTH),
            CORNER_OFFSET - 5, 10 + 2 * FONT_GLYPH_WIDTH,
            10 + FONT_GLYPH_HEIGHT);

        LCD.SetFontColor(WHITE);
        LCD.WriteAt((int)(game->combo),
                    LCD_WIDTH - CORNER_OFFSET -
                        FONT_GLYPH_WIDTH * (int)(log10(game->combo) + 1),
                    CORNER_OFFSET);
        LCD.DrawHorizontalLine(CORNER_OFFSET + FONT_GLYPH_HEIGHT + 2,
                               LCD_WIDTH - CORNER_OFFSET,
                               LCD_WIDTH - CORNER_OFFSET +
                                   FONT_GLYPH_WIDTH * 2 / COMBO_DUR *
                                       (TimeNow() - combo_time - COMBO_DUR));
    }

    // remove physics objects if they've gone out of bounds or otherwise need to
    // be destroyed
    remove_if_foreach(apples);
    remove_if_foreach(bananas);
    remove_if_foreach(oranges);
    remove_if_foreach(cherries);
    remove_if_foreach(strawberries);
    remove_if_foreach(pineapples);
    remove_if_foreach(bombs);
    remove_if_foreach(fruit_shards);

    // update physics objects
    update_foreach(alpha, apples);
    update_foreach(alpha, bananas);
    update_foreach(alpha, oranges);
    update_foreach(alpha, cherries);
    update_foreach(alpha, strawberries);
    update_foreach(alpha, pineapples);
    update_foreach(alpha, bombs);

    // update and draw knife
    knife.update();

    // update and draw fruit shards
    update_foreach(alpha, fruit_shards);

    // End the game if the game has gone on for max duration
    if (GAME_DURATION <= TimeNow() - time_started) {
        end();
    }
}