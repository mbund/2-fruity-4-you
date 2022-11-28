#pragma once

/// @file game.h
/// Main gameplay

#include <memory>
#include <vector>

#include "knife.h"
#include "throwable.h"
#include "util.h"

/// Main Scene for playing the game
class Game : public Scene {
   public:
    /// Default constructor
    Game();

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha);

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    void physics_update(double t, double dt);

    /// Start a new game
    /// @param bomb_probability probability [0, 1] that any given thrown object
    /// will be a bomb
    /// @param multiplier score multiplier (rewarding higher difficulties)
    void start(float bomb_probability, float multiplier);

    std::vector<std::unique_ptr<Apple>> apples;
    std::vector<std::unique_ptr<Bananas>> bananas;
    std::vector<std::unique_ptr<FruitShard>> fruit_shards;

   private:
    float bomb_probability;
    float multiplier;
    double time_started;
    Knife knife;

    float x;
};

/// Global variable to hold the state of the game
inline auto game = std::make_shared<Game>();