/// @file ui.cpp
/// Implementation for the UI

#include "ui.h"
#include <FEHLCD.h>
#include <iostream>

UIButton::UIButton(std::string text, UIPosition pos)
    : text(text),
      pos(pos),
      state(ButtonState::None),
      on_button_up([]() {}),
      on_button_down([]() {}),
      on_button_hover([]() {}),
      on_button_leave([]() {}),
      padding_x(10),
      padding_y(8),
      box(UIBox(pos,
                text.length() * FONT_GLYPH_WIDTH + padding_x * 2,
                FONT_GLYPH_HEIGHT + padding_y * 2, pos.screen_anchor)) {}

void UIButton::update() {
    int touchX, touchY;
    bool pressed = LCD.Touch(&touchX, &touchY);
    bool in_bounds = touchX >= box.get_x() && touchX <= box.get_x() + box.width &&
                     touchY >= box.get_y() && touchY <= box.get_y() + box.height;

    if (in_bounds && pressed && state != ButtonState::Down) {
        state = ButtonState::Down;
        on_button_down();
    } else if (!pressed && state == ButtonState::Down) {
        state = ButtonState::Up;
        on_button_up();
    } else if (in_bounds && !pressed && state != ButtonState::Hover) {
        state = ButtonState::Hover;
        on_button_hover();
    } else if (!in_bounds && state != ButtonState::None) {
        state = ButtonState::None;
        on_button_leave();
    }

    box.update();
    LCD.WriteAt(text.c_str(), box.get_x() + padding_x, box.get_y() + padding_y);
}

void UIButton::bind_on_button_up(std::function<void()> f) {
    on_button_up = f;
}

void UIButton::bind_on_button_down(std::function<void()> f) {
    on_button_down = f;
}

void UIButton::bind_on_button_hover(std::function<void()> f) {
    on_button_hover = f;
}

void UIButton::bind_on_button_leave(std::function<void()> f) {
    on_button_leave = f;
}

UIPosition::UIPosition(uint64_t absolute_x,
                       uint64_t absolute_y,
                       Anchor anchor)
    : screen_anchor(anchor) {
    if (anchor == Anchor::TopLeft) {
        x = absolute_x;
        y = absolute_y;
    } else if (anchor == Anchor::TopRight) {
        x = LCD_WIDTH - absolute_x;
        y = absolute_y;
    } else if (anchor == Anchor::BottomLeft) {
        x = absolute_x;
        y = LCD_HEIGHT - absolute_y;
    } else if (anchor == Anchor::BottomRight) {
        x = LCD_WIDTH - absolute_x;
        y = LCD_HEIGHT - absolute_y;
    } else if (anchor == Anchor::Center) {
        x = LCD_WIDTH / 2 + absolute_x;
        y = LCD_HEIGHT / 2 + absolute_y;
    }
}

UIBox::UIBox(UIPosition pos, uint64_t width, uint64_t height, UIPosition::Anchor anchor)
    : pos(pos), width(width), height(height), anchor(anchor) {}

uint64_t UIBox::get_x() {
    switch (anchor) {
        case UIPosition::TopLeft:
        case UIPosition::BottomLeft:
            return pos.x;
        case UIPosition::TopRight:
        case UIPosition::BottomRight:
            return pos.x - width;
        case UIPosition::Center:
            return pos.x - width / 2;
    }
}

uint64_t UIBox::get_y() {
    switch (anchor) {
        case UIPosition::TopLeft:
        case UIPosition::TopRight:
            return pos.y;
        case UIPosition::BottomLeft:
        case UIPosition::BottomRight:
            return pos.y - height;
        case UIPosition::Center:
            return pos.y - height / 2;
    }
}

void UIBox::update() {
    uint64_t x = get_x();
    uint64_t y = get_y();

    LCD.SetFontColor(FEHLCD::FEHLCDColor::Black);
    LCD.FillRectangle(x, y, width, height);
    LCD.SetFontColor(FEHLCD::FEHLCDColor::White);
    LCD.DrawRectangle(x, y, width, height);
}
