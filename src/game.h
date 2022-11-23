#pragma once

/// @file game.h
/// Main gameplay

#include <memory>
#include <vector>

#include "knife.h"
#include "throwable.h"
#include "util.h"

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

    std::vector<Apple> apples;

   private:
    float bomb_probability;
    float multiplier;
    double time_started;
    Knife knife;

    float x;
};

/// Global variable to hold the state of the game
inline auto game = std::make_shared<Game>();