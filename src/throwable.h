#pragma once

/// @file throwable.h
/// Throwable objects with physics

#include <memory>
#include <vector>

#include "FEHLCD.h"

#include "image.h"
#include "util.h"

/// Throwable apple fruit
class Apple {
   public:
    /// Default constructor
    Apple(Vector2 pos, float mass);

    /// Update and render the apple
    /// @param dt delta time, the time elapsed since the start of the frame
    void update(float dt);

    /// Cut the apple in half
    void cut();

    /// Apply a force to the throwable object
    /// @param force force vector to apply
    void add_force(Vector2 force);

    /// Detect collision between the apple and a line
    /// @param p1 first point on line
    /// @param p2 second point on line
    void collision(Vector2 p1, Vector2 p2);

   protected:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    FEHLCD::FEHLCDColor color = FEHLCD::FEHLCDColor::Red;
    std::shared_ptr<Image> image;
};

/// Throwable bomb
class Bomb {
   public:
    /// Default constructor
    Bomb();

    /// Update and render the bomb
    /// @param dt delta time, the time elapsed since the start of the frame
    void update(float dt);

    /// Cut the apple in half
    void cut();
};