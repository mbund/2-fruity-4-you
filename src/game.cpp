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

#define COMBO_DUR 2.0

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
    
    int time_left= (int)(30-(TimeNow()-time_started))+1;
    if(time_left>=10)
        LCD.WriteAt(time_left, 10, 10);
    else{
        LCD.WriteAt(0, 10, 10);
        LCD.WriteAt(time_left, 10+FONT_GLYPH_WIDTH, 10);
    }
    if(TimeNow()-comboTime>COMBO_DUR){
        combo=0;
    }

    int color1=ORANGERED;
    int color2=0x2145A8+game->combo*0x050000;

    if(game->combo !=0 ){
        LCD.SetFontColor(std::min(color1, color2));
        LCD.WriteAt((int)(game->combo), LCD_WIDTH-10-FONT_GLYPH_WIDTH*(int)(log10(game->combo)+1), 10);
        LCD.DrawHorizontalLine(10+FONT_GLYPH_HEIGHT+4,LCD_WIDTH-10, LCD_WIDTH-10+FONT_GLYPH_WIDTH*2/COMBO_DUR*(TimeNow()-comboTime-COMBO_DUR));
    }


    int randSpawn = rand_range(0, 120 + 1.5*bomb_probability);
    float randX = rand_range(20, LCD_WIDTH - 20);
    float randForce = rand_range(-50, 80000);
    paused =(30<=TimeNow()-time_started);

    if (randX > LCD_WIDTH / 2)
        randForce *= -1.0;

    Vector2 pos = {randX, LCD_HEIGHT + 20};
    Vector2 first_force = {randForce, rand_range(-360000, -260000)};

    if (randSpawn > 120 - 2) {
        auto bomb = std::make_unique<Bomb>(pos);
        bomb->add_force(first_force);
        bombs.push_back(std::move(bomb));
    } else if (randSpawn == 0) {
        auto apple = std::make_unique<Apple>(pos);
        apple->add_force(first_force);
        apples.push_back(std::move(apple));
    } else if (randSpawn == 1) {
        auto banana = std::make_unique<Bananas>(pos);
        banana->add_force(first_force);
        bananas.push_back(std::move(banana));
    } else if(randSpawn==2){
        auto apple = std::make_unique<Apple>(pos);
        apple->add_force(first_force);
        apples.push_back(std::move(apple));
    } else if(randSpawn ==3){
        auto banana = std::make_unique<Bananas>(pos);
        banana->add_force(first_force);
        bananas.push_back(std::move(banana));
    }

    remove_if_foreach(apples);
    remove_if_foreach(bananas);
    remove_if_foreach(bombs);
    remove_if_foreach(fruit_shards);

    update_foreach(alpha, apples);
    update_foreach(alpha, bananas);
    update_foreach(alpha, bombs);

    if (!paused) {
        knife.update();

        update_foreach(alpha, fruit_shards);
    }
    

    // screen wipe clear the screen
    if (paused) {
        end_game->end(points);
        current_scene = end_game;
    }
}