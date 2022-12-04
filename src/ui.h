#pragma once

/// @file ui.h
/// User interface components and utilities
/// Author: Mark

#include <functional>
#include <memory>
#include <string>

/// Width in pixels of a single character in the default font
const uint64_t FONT_GLYPH_WIDTH = 12;
/// Height in pixels of a single character in the default font
const uint64_t FONT_GLYPH_HEIGHT = 15;

/// Width in pixels of the LCD (display)
// #define LCD_WIDTH 320
const uint64_t LCD_WIDTH = 320;

/// Height in pixels of the LCD (display)
const uint64_t LCD_HEIGHT = 240;

/// Representation of a position in the UI
class UIPosition {
   public:
    /// Anchor positions of a UI component
    typedef enum {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center,
    } Anchor;

    /// Create a UIPosition from pixel coordinates and a screen anchor position
    /// to offset away from
    /// @param absolute_x x coordinate in pixels
    /// @param absolute_y y coordinate in pixel
    /// @param screen_anchor anchor to offset absolute_x and absolute_y away
    /// from the screen
    UIPosition(uint64_t absolute_x,
               uint64_t absolute_y,
               Anchor screen_anchor = Anchor::TopLeft);

   private:
    uint64_t x, y;
    Anchor screen_anchor;

    friend class UIButton;
    friend class UIBox;
};

/// Box UI component
class UIBox {
   public:
    /// Constructor to create a UIBox UI component
    /// @param pos position to place the box
    /// @param width width in pixels of the box
    /// @param height height in pixels of the box
    /// @param anchor anchor inside the box to expand out from
    UIBox(UIPosition pos,
          uint64_t width,
          uint64_t height,
          UIPosition::Anchor anchor = UIPosition::Anchor::Center);

    /// Update and render the UI component
    void update();

    /// Retrieve the x coordinate of the upper left corner of the box in raw
    /// screen pixel coordinates
    /// @return x coordinate of the upper left corner of the box in screen pixel
    /// coordinates
    uint64_t get_x();

    /// Retrieve the y coordinate of the upper left corner of the box in raw
    /// screen pixel coordinates
    /// @return y coordinate of the upper left corner of the box in screen pixel
    /// coordinates
    uint64_t get_y();

    /// Sets the background color of the box to the given color
    /// @param background_color the background color to set it to
    void set_background_color(unsigned int background_color);

    /// Width in pixels of the box
    uint64_t width;

    /// Height in pixels of the box
    uint64_t height;

   private:
    UIPosition pos;
    UIPosition::Anchor anchor;
    unsigned int background_color;

    friend class UIButton;
};

/// Button UI component
class UIButton {
   public:
    /// Create a simple button
    /// @param text single line of text for the contents of the button
    /// @param pos position of the button
    UIButton(std::string text,
             UIPosition pos,
             uint64_t padding_x = 10,
             uint64_t padding_y = 8);

    /// Checks for mouse events interacing with the button and will fire off the
    /// corresponding event. Also renders the button.
    /// @see bind the on button up event bind_on_button_up
    /// @see bind the on button down event bind_on_button_down
    /// @see bind the on button hover event bind_on_button_hover
    /// @see bind the on button leave event bind_on_button_leave
    void update();

    /// Binds the on button up event to a user function
    /// @param f callback which will be fired when the button is released
    void bind_on_button_up(std::function<void()> f);

    /// Binds the on button down event to a user function
    /// @param f callback which will be fired when the button is pressed down
    void bind_on_button_down(std::function<void()> f);

    /// Binds the on button hover event to a user function
    /// @param f callback which will be fired when the button is hovered over
    void bind_on_button_hover(std::function<void()> f);

    /// Binds the on button leave event to a user function
    /// @param f callback which will be fired when the button is no longer being
    /// hovered over
    void bind_on_button_leave(std::function<void()> f);

   private:
    typedef enum {
        Down,
        Up,
        Hover,
        None,
    } ButtonState;

    std::string text;
    UIPosition pos;
    std::unique_ptr<UIBox> box;
    ButtonState state;
    std::function<void()> on_button_up;
    std::function<void()> on_button_down;
    std::function<void()> on_button_hover;
    std::function<void()> on_button_leave;
    uint64_t padding_x, padding_y;
};
