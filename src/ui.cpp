#include "ui.h"
#include <FEHLCD.h>
#include <iostream>

#define FONT_GLYPH_WIDTH 12
#define FONT_GLYPTH_HEIGHT 15

UIButton::UIButton(std::string text, uint64_t x, uint64_t y)
    : text(text),
      x(x),
      y(y),
      state(ButtonState::None),
      on_button_up([]() {}),
      on_button_down([]() {}) {}

void UIButton::update() {
    uint64_t padding_x = 10;
    uint64_t padding_y = 8;

    uint64_t button_x = x - 1 - padding_x;
    uint64_t button_y = y - padding_y;
    uint64_t width = text.length() * FONT_GLYPH_WIDTH + padding_x * 2;
    uint64_t height = FONT_GLYPTH_HEIGHT + padding_y * 2;

    int touchX, touchY;
    bool pressed = LCD.Touch(&touchX, &touchY);
    if (touchX >= button_x && touchX <= button_x + width &&
        touchY >= button_y && touchY <= button_y + height) {
        if (state == ButtonState::Hover && pressed) {
            state = ButtonState::Down;
            on_button_down();
        } else if (state == ButtonState::Down && !pressed) {
            state = ButtonState::Up;
            on_button_up();
        } else if (state == ButtonState::Up && !pressed) {
            state = ButtonState::Hover;
        } else if (state == ButtonState::None && !pressed) {
            state = ButtonState::Hover;
        }
    } else {
        state = ButtonState::None;
    }

    LCD.DrawRectangle(button_x, button_y, width, height);
    LCD.WriteAt(text.c_str(), x, y);
}

void UIButton::bind_on_button_up(std::function<void()> f) {
    on_button_up = f;
}

void UIButton::bind_on_button_down(std::function<void()> f) {
    on_button_down = f;
}
