/// @file endgame.cpp
/// @author Mark Bundschuh
/// @brief End game and keyboard input implementation

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "FEHLCD.h"

#include "endgame.h"
#include "menu.h"
#include "ui.h"
#include "util.h"

EndGame::EndGame() {
    const uint64_t Y_POS = 100;
    const uint64_t BUTTON_WIDTH = 20;
    const uint64_t BUTTON_HEIGHT = 24;
    const uint64_t MAX_WIDTH = row1.size() * BUTTON_WIDTH;
    const uint64_t LEFTMOST = (LCD_WIDTH / 2) - (MAX_WIDTH / 2);

    const std::string keyrow1 = "QWERTYUIOP";
    for (size_t i = 0; i < row1.size(); i++) {
        row1[i] = std::make_unique<UIButton>(
            std::string(1, keyrow1[i]),
            UIPosition(LEFTMOST + i * BUTTON_WIDTH, Y_POS), 3, 3);
        row1[i]->bind_on_button_up([this, keyrow1, i]() {
            if (index < 3)
                name[index++] = keyrow1[i];
        });
    }

    const std::string keyrow2 = "ASDFGHJKL";
    for (size_t i = 0; i < row2.size(); i++) {
        row2[i] = std::make_unique<UIButton>(
            std::string(1, keyrow2[i]),
            UIPosition(LEFTMOST + 5 + i * BUTTON_WIDTH, Y_POS + BUTTON_HEIGHT),
            3, 3);
        row2[i]->bind_on_button_up([this, keyrow2, i]() {
            if (index < 3)
                name[index++] = keyrow2[i];
        });
    }

    const std::string keyrow3 = "ZXCVBNM";
    for (size_t i = 0; i < row3.size(); i++) {
        row3[i] = std::make_unique<UIButton>(
            std::string(1, keyrow3[i]),
            UIPosition(LEFTMOST + 20 + i * BUTTON_WIDTH,
                       Y_POS + BUTTON_HEIGHT * 2),
            3, 3);
        row3[i]->bind_on_button_up([this, keyrow3, i]() {
            if (index < 3)
                name[index++] = keyrow3[i];
        });
    }

    clear_button = std::make_unique<UIButton>(
        "Clear", UIPosition(20, 20, UIPosition::BottomLeft));
    clear_button->bind_on_button_up([this]() {
        index = 0;
        name.assign("___");
    });

    confirm_button = std::make_unique<UIButton>(
        "Confirm", UIPosition(20, 20, UIPosition::BottomRight));
    confirm_button->bind_on_button_up([this]() {
        std::replace(name.begin(), name.end(), '_', ' ');
        menu->leaderboard.add_entry({.name = name, .points = points});
        current_scene = menu;
    });

    background = image_repository->load_image("assets/background-menu.png");
}

void EndGame::update(double alpha) {
    background->render(LCD_WIDTH / 2, LCD_HEIGHT / 2, 0);

    LCD.SetFontColor(WHITE);
    const auto points_str = std::to_string(points);
    LCD.WriteAt(
        points_str.c_str(),
        (LCD_WIDTH / 2.0) - (points_str.size() * FONT_GLYPH_WIDTH / 2.0),
        FONT_GLYPH_HEIGHT);
    LCD.WriteAt("Enter Name", (LCD_WIDTH / 2.0) - (5 * FONT_GLYPH_WIDTH),
                FONT_GLYPH_HEIGHT * 2 + 4 * 1);
    LCD.WriteAt(name.c_str(), (LCD_WIDTH / 2.0) - (1.5 * FONT_GLYPH_WIDTH),
                FONT_GLYPH_HEIGHT * 3 + 4 * 2);

    std::for_each(row1.begin(), row1.end(), [](auto& a) { a->update(); });
    std::for_each(row2.begin(), row2.end(), [](auto& a) { a->update(); });
    std::for_each(row3.begin(), row3.end(), [](auto& a) { a->update(); });
    confirm_button->update();
    clear_button->update();
}

void EndGame::end(uint32_t points) {
    this->points = points;
    index = 0;
    name.assign("___");
}
