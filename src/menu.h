#include <string>
#include <vector>

#include "ui.h"

class Credits {
   public:
    Credits();
    void show();
    void update();

   private:
    UIButton close_button;
    UIBox box;
    bool is_showing;
};

class Leaderboard {
   public:
    Leaderboard();
    void update();

    struct Entry {
        std::string name;
        uint64_t points;
    };

   private:
    std::vector<Entry> entries;
    UIBox box;
};

class Menu {
   public:
    Menu();
    void update();

   private:
    Credits credits;
    UIButton show_credits_button;
    UIButton quit_button;
    UIButton play_easy;
    UIButton play_medium;
    UIButton play_hard;
    Leaderboard leaderboard;
};
