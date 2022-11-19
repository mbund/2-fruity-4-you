#include <cstdint>
#include <functional>
#include <string>

class UIButton {
   public:
    UIButton(std::string text, uint64_t x, uint64_t y);
    void update();
    void bind_on_button_up(std::function<void()> f);
    void bind_on_button_down(std::function<void()> f);

    typedef enum {
        Down,
        Up,
        Hover,
        None,
    } ButtonState;

   private:
    uint64_t x, y;
    std::string text;
    ButtonState state;
    std::function<void()> on_button_up;
    std::function<void()> on_button_down;
};
