#pragma once

/// @file endgame.h
/// Scene at end of game to input name and return to main menu
///Author: Mark

#include <array>
#include <memory>

#include "image.h"
#include "ui.h"
#include "util.h"



/// End game Scene
class EndGame : public Scene {
   public:
    /// Default constructor
    EndGame();

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha);

    /// End the current game
    /// @param points point value from the game
    void end(uint32_t points);

   private:
    uint32_t points;
    std::string name;
    size_t index = 0;
    std::array<std::unique_ptr<UIButton>, 10> row1;
    std::array<std::unique_ptr<UIButton>, 9> row2;
    std::array<std::unique_ptr<UIButton>, 7> row3;
    std::unique_ptr<UIButton> clear_button;
    std::unique_ptr<UIButton> confirm_button;
    std::shared_ptr<Image> background;
};

/// Global variable to hold the state of the end game
inline auto end_game = std::make_shared<EndGame>();