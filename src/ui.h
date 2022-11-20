/// @file ui.h
/// User interface utilities

#include <cstdint>
#include <functional>
#include <string>

#define FONT_GLYPH_WIDTH 12
#define FONT_GLYPH_HEIGHT 15

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

class UIPosition {
   public:
    typedef enum {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center,
    } Anchor;

    UIPosition(uint64_t absolute_x,
               uint64_t absolute_y,
               Anchor anchor = Anchor::TopLeft);

   private:
    uint64_t x, y;
    Anchor screen_anchor;

    friend class UIButton;
    friend class UIBox;
};

class UIBox {
   public:
    UIBox(UIPosition pos, uint64_t width, uint64_t height, UIPosition::Anchor anchor = UIPosition::Anchor::Center);
    void update();

    uint64_t get_x();
    uint64_t get_y();
    uint64_t width, height;

   private:
    UIPosition pos;
    UIPosition::Anchor anchor;

    friend class UIButton;
};

class UIButton {
   public:
    UIButton(std::string text, UIPosition pos);
    void update();
    void bind_on_button_up(std::function<void()> f);
    void bind_on_button_down(std::function<void()> f);
    void bind_on_button_hover(std::function<void()> f);
    void bind_on_button_leave(std::function<void()> f);

   private:
    typedef enum {
        Down,
        Up,
        Hover,
        None,
    } ButtonState;

    UIPosition pos;
    uint64_t padding_x, padding_y;
    UIBox box;
    std::string text;
    ButtonState state;
    std::function<void()> on_button_up;
    std::function<void()> on_button_down;
    std::function<void()> on_button_hover;
    std::function<void()> on_button_leave;
};

