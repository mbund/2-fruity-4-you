#pragma once

/// @file menu.h
/// Components for the menu and the menu scene

#include <memory>
#include <string>
#include <vector>

#include "ui.h"
#include "util.h"

/// Main menu Scene which shows the credits
class Credits : public Scene {
   public:
    /// Default constructor
    Credits();

    /// Update and render the scene
    /// @param dt delta time, the time elapsed since the start of the frame
    virtual void update(float dt);

   private:
    std::unique_ptr<UIButton> close_button;
    std::unique_ptr<UIBox> box;
};

/// UI component for displaying and interacting with the leaderboard
class Leaderboard {
   public:
    /// An entry of the Leaderboard
    struct Entry {
        std::string name;
        uint64_t points;
    };

    /// Default constructor
    Leaderboard();

    /// Update and render the component
    void update();

    /// Adds an entry into its sorted place in the Leaderboard
    /// @param entry entry to add
    void addEntry(Entry entry);

   private:
    std::vector<Entry> entries;
    std::unique_ptr<UIBox> box;
};

/// Menu scene
class Menu : public Scene {
   public:
    /// Default constructor
    Menu();

    /// Update and render the scene
    /// @param dt delta time, the time elapsed since the start of the frame
    void update(float dt);

    /// Internal leaderboard of the menu, used by others to add entries to the
    /// leaderboard
    Leaderboard leaderboard;

   private:
    std::unique_ptr<UIButton> show_credits_button;
    std::unique_ptr<UIButton> quit_button;
    std::unique_ptr<UIButton> play_easy;
    std::unique_ptr<UIButton> play_medium;
    std::unique_ptr<UIButton> play_hard;
};

/// Global variable to hold the state of the menu
inline auto menu = std::make_shared<Menu>();

/// Global variable to hold the state of the credits
inline auto credits = std::make_shared<Credits>();