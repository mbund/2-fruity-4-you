/// @file ui.cpp
/// Implementation for the UI

#include <iostream>
#include <memory>

#include <FEHLCD.h>

#include "ui.h"
#include "util.h"

UIButton::UIButton(std::string text, UIPosition pos)
    : text(text),
      pos(pos),
      state(ButtonState::None),
      on_button_up([]() {}),
      on_button_down([]() {}),
      on_button_hover([this]() {
          this->box->set_background_color(FEHLCD::FEHLCDColor::Gray);
      }),
      on_button_leave([this]() {
          this->box->set_background_color(FEHLCD::FEHLCDColor::Black);
      }),
      padding_x(10),
      padding_y(8) {
    uint64_t width = text.length() * FONT_GLYPH_WIDTH + padding_x * 2;
    uint64_t height = FONT_GLYPH_HEIGHT + padding_y * 2;
    box = std::make_unique<UIBox>(pos, width, height, pos.screen_anchor);
}

void UIButton::update() {
    bool in_bounds = (uint64_t)touchX >= box->get_x() &&
                     (uint64_t)touchX <= box->get_x() + box->width &&
                     (uint64_t)touchY >= box->get_y() &&
                     (uint64_t)touchY <= box->get_y() + box->height;

    if (in_bounds && touchPressed && state != ButtonState::Down) {
        state = ButtonState::Down;
        on_button_down();
    } else if (!touchPressed && state == ButtonState::Down) {
        state = ButtonState::Up;
        on_button_up();
    } else if (in_bounds && !touchPressed && state != ButtonState::Hover) {
        state = ButtonState::Hover;
        on_button_hover();
    } else if (!in_bounds && state != ButtonState::None) {
        state = ButtonState::None;
        on_button_leave();
    }

    box->update();
    LCD.WriteAt(text.c_str(), box->get_x() + padding_x,
                box->get_y() + padding_y);
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

void UIBox::set_background_color(FEHLCD::FEHLCDColor background_color) {
    this->background_color = background_color;
}

void default_on_button_hover() {}

UIPosition::UIPosition(uint64_t absolute_x,
                       uint64_t absolute_y,
                       Anchor screen_anchor)
    : screen_anchor(screen_anchor) {
    if (screen_anchor == Anchor::TopLeft) {
        x = absolute_x;
        y = absolute_y;
    } else if (screen_anchor == Anchor::TopRight) {
        x = LCD_WIDTH - absolute_x;
        y = absolute_y;
    } else if (screen_anchor == Anchor::BottomLeft) {
        x = absolute_x;
        y = LCD_HEIGHT - absolute_y;
    } else if (screen_anchor == Anchor::BottomRight) {
        x = LCD_WIDTH - absolute_x;
        y = LCD_HEIGHT - absolute_y;
    } else if (screen_anchor == Anchor::Center) {
        x = LCD_WIDTH / 2 + absolute_x;
        y = LCD_HEIGHT / 2 + absolute_y;
    }
}

UIBox::UIBox(UIPosition pos,
             uint64_t width,
             uint64_t height,
             UIPosition::Anchor anchor)
    : width(width), height(height), pos(pos), anchor(anchor) {}

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

    // supress -Wreturn-type
    return 0;
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

    // supress -Wreturn-type
    return 0;
}

void UIBox::update() {
    uint64_t x = get_x();
    uint64_t y = get_y();

    LCD.SetFontColor(background_color);
    LCD.FillRectangle(x, y, width, height);
    LCD.SetFontColor(FEHLCD::FEHLCDColor::White);
    LCD.DrawRectangle(x, y, width, height);
}
