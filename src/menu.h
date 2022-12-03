#pragma once

/// @file menu.h
/// Components for the menu and the menu scene

#include <memory>
#include <string>
#include <vector>

#include "image.h"
#include "ui.h"
#include "util.h"

/// Main menu Scene which shows the credits
class Credits final : public Scene {
   public:
    /// Default constructor
    Credits();

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    virtual void update(double alpha);

   private:
    std::unique_ptr<UIButton> close_button;
    std::unique_ptr<UIBox> box;
    std::shared_ptr<Image> background;
};

/// Main menu Scene which shows the instructions for how to play the game
class Instructions final : public Scene {
   public:
    /// Default constructor
    Instructions();

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    virtual void update(double alpha);

   private:
    std::unique_ptr<UIButton> close_button;
    std::unique_ptr<UIBox> box;
    std::shared_ptr<Image> background;
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
    void add_entry(Entry entry);

   private:
    std::vector<Entry> entries;
    std::unique_ptr<UIBox> box;
};

/// Main menu Scene
class Menu final : public Scene {
   public:
    /// Default constructor
    Menu();

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha);

    /// Internal leaderboard of the menu, used by others to add entries to the
    /// leaderboard
    Leaderboard leaderboard;

   private:
    std::unique_ptr<UIButton> show_credits_button;
    std::unique_ptr<UIButton> show_instructions_button;
    std::unique_ptr<UIButton> quit_button;
    std::unique_ptr<UIButton> play_easy;
    std::unique_ptr<UIButton> play_medium;
    std::unique_ptr<UIButton> play_hard;
    std::shared_ptr<Image> background;
};

/// Global variable to hold the state of the menu
inline auto menu = std::make_shared<Menu>();

/// Global variable to hold the state of the credits
inline auto credits = std::make_shared<Credits>();

/// Global variable to hold the state of the instructions
inline auto instructions = std::make_shared<Instructions>();