/// @file menu.cpp
/// Implementation for menu components

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <FEHLCD.h>

#include "game.h"
#include "menu.h"
#include "src/image.h"
#include "ui.h"
#include "util.h"

Credits::Credits() {
    close_button = std::make_unique<UIButton>(
        "Close", UIPosition(30, 30, UIPosition::Anchor::BottomRight));
    box = std::make_unique<UIBox>(UIPosition(0, 0, UIPosition::Center),
                                  LCD_WIDTH - 10 * 2, LCD_HEIGHT - 10 * 2);

    close_button->bind_on_button_up([&]() { current_scene = menu; });

    background = image_repository->load_image("assets/background-menu.png");
}

void Credits::update(double alpha) {
    background->render(LCD_WIDTH / 2, LCD_HEIGHT / 2, 0);

    box->update();
    constexpr uint64_t inner_padding = 10;
    uint64_t x = box->get_x() + inner_padding, y = box->get_y() + inner_padding;
    std::string title = "2 Fruity 4 you";
    LCD.WriteAt(title.c_str(), x, y);
    LCD.DrawHorizontalLine(y + FONT_GLYPH_HEIGHT + 1, x,
                           x + title.length() * FONT_GLYPH_WIDTH);

    constexpr uint64_t SPACING = FONT_GLYPH_HEIGHT + 2;
    LCD.WriteAt("Mark Bundschuh", x, y + SPACING * 2);
    LCD.WriteAt("John Ulm", x, y + SPACING * 3);

    LCD.WriteAt("Autumn 2022", x, y + SPACING * 5);
    LCD.WriteAt("ENGR 1281.02H (6989)", x, y + SPACING * 6);

    close_button->update();
}

Leaderboard::Leaderboard() {
    box = std::make_unique<UIBox>(
        UIPosition(10, 10, UIPosition::Anchor::TopRight), LCD_WIDTH / 3,
        LCD_HEIGHT - 10 * 2, UIPosition::Anchor::TopRight);

    // read in leaderboard from file
    std::ifstream leaderboard_csv("leaderboard.csv");
    std::string name, points;
    while (getline(leaderboard_csv, name, ',')) {
        getline(leaderboard_csv, points);
        add_entry({.name = name, .points = std::stoull(points)});
    }
}

void Leaderboard::add_entry(Entry entry) {
    // insert into sorted vector
    entries.insert(
        std::upper_bound(entries.begin(), entries.end(), entry,
                         [](Entry a, Entry b) { return a.points > b.points; }),
        entry);

    // write leaderboard to file
    std::ofstream leaderboard_csv("leaderboard.csv");
    std::for_each(
        entries.begin(), entries.end(), [&leaderboard_csv](Entry& entry) {
            leaderboard_csv << entry.name << "," << entry.points << "\n";
        });
}

void Leaderboard::update() {
    uint64_t inner_padding = 5;

    box->update();
    uint64_t name_x = box->get_x() + inner_padding - 1;

    constexpr uint64_t SPACING = FONT_GLYPH_HEIGHT + 2;

    // UI overflows with more than 12 entries
    for (size_t i = 0; i < std::min((uint32_t)entries.size(), 12u); i++) {
        uint64_t y = box->get_y() + inner_padding + SPACING * i;
        auto num = std::to_string(entries[i].points);
        uint64_t num_x = box->get_x() + box->width - inner_padding -
                         FONT_GLYPH_WIDTH * num.length();

        LCD.WriteAt(entries[i].name.c_str(), name_x, y);
        LCD.WriteAt(num.c_str(), num_x, y);
    }
}

Menu::Menu() {
    show_credits_button = std::make_unique<UIButton>(
        "Credits", UIPosition(10, 10, UIPosition::Anchor::BottomLeft));
    quit_button = std::make_unique<UIButton>(
        "Quit", UIPosition(10, 50, UIPosition::Anchor::BottomLeft));
    play_easy = std::make_unique<UIButton>(
        "Play (easy 1x)", UIPosition(10, 40, UIPosition::Anchor::TopLeft));
    play_medium = std::make_unique<UIButton>(
        "Play (med  2x)", UIPosition(10, 80, UIPosition::Anchor::TopLeft));
    play_hard = std::make_unique<UIButton>(
        "Play (hard 3x)", UIPosition(10, 120, UIPosition::Anchor::TopLeft));

    background = image_repository->load_image("assets/background-menu.png");

    show_credits_button->bind_on_button_up([&]() { current_scene = credits; });
    quit_button->bind_on_button_up([&]() { exit(0); });
    play_easy->bind_on_button_up([&]() {
        current_scene = game;
        game->start(0.1, 1);
    });
    play_medium->bind_on_button_up([&]() {
        current_scene = game;
        game->start(0.2, 2);
    });
    play_hard->bind_on_button_up([&]() {
        current_scene = game;
        game->start(0.3, 3);
    });
}

void Menu::update(double alpha) {
    uint64_t x = 20, y = 20;
    std::string title = "2 Fruity 4 you";
    LCD.WriteAt(title.c_str(), x, y);
    LCD.DrawHorizontalLine(y + FONT_GLYPH_HEIGHT + 1, x,
                           x + title.length() * FONT_GLYPH_WIDTH);

    background->render(LCD_WIDTH / 2, LCD_HEIGHT / 2, 0);
    show_credits_button->update();
    quit_button->update();
    play_easy->update();
    play_medium->update();
    play_hard->update();
    leaderboard.update();
}
