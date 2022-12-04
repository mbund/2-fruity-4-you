#pragma once

/// @file game.h
/// Main gameplay
///Author: Mark unless stated otherwise

#include <memory>
#include <vector>

#include "image.h"
#include "knife.h"
#include "throwable.h"
#include "util.h"



/// Main Scene for playing the game
class Game final : public Scene {
   public:
    /// Default constructor
    Game();

    ///Author: John
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

    /// End the current game
    void end();

    /// Check if the line segment formed by p1 and p2 intersects with any fruits
    /// or bombs, and handles collision and effects accordingly
    /// @param p1 Screenspace coordinate vector for the first endpoint of the
    /// line segment
    /// @param p2 Screenspace coordinate vector for the last endpoint of the
    /// line segment
    void collide_with_knife(Vector2 p1, Vector2 p2);

    /// All fruit shards which need to be updated and rendered
    std::vector<std::unique_ptr<FruitShard>> fruit_shards;

    /// Number of points scored in the game
    uint32_t points;

    /// Current combo in the game
    uint32_t combo;

    /// Time that the most recent fruit was cut
    double combo_time;

    /// score multiplier (should reward higher difficulties)
    float multiplier;

   private:
    /// Duration of the game in seconds
    const uint32_t GAME_DURATION = 30;
    /// value that determines rate that bombs spawn
    float bomb_probability;
    /// start time
    double time_started;

    Knife knife;

    std::shared_ptr<Image> background;

    std::vector<std::unique_ptr<Apple>> apples;
    std::vector<std::unique_ptr<Bananas>> bananas;
    std::vector<std::unique_ptr<Orange>> oranges;
    std::vector<std::unique_ptr<Cherries>> cherries;
    std::vector<std::unique_ptr<Strawberry>> strawberries;
    std::vector<std::unique_ptr<Pineapple>> pineapples;
    std::vector<std::unique_ptr<Bomb>> bombs;
};

/// Global variable to hold the state of the game
inline auto game = std::make_shared<Game>();