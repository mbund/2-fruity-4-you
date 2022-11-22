#pragma once

/// @file game.h
/// Main gameplay

#include <memory>

#include "main.h"

/// Game scene
class Game : public Scene {
   public:
    /// Default constructor
    Game();

    /// Update and render the scene
    /// @param dt delta time, the time elapsed since the start of the frame
    void update(float dt);

    /// Start a new game
    /// @param bomb_probability probability [0, 1] that any given thrown object
    /// will be a bomb
    /// @param multiplier score multiplier (rewarding higher difficulties)
    void start(float bomb_probability, float multiplier);

   private:
    float bomb_probability;
    float multiplier;

    float x;
};

/// Global variable to hold the state of the game
inline auto game = std::make_shared<Game>();