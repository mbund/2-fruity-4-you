#pragma once

/// @file util.h
/// Global variables, definitions, and miscellaneous utilities

// Use c++17 inline for global variables

#include <memory>
#include <random>

#include "FEHRandom.h"

/// Global variable for whether the screen is currently being touched
inline bool touchPressed;
/// Global variable for the current touched x coordinate
inline int touchX;
/// Global variable for the current touched y coordinate
inline int touchY;

// Get a random uniformly distributed random number in the inclusive range
// [lower, upper]
float rand_range(float lower, float upper);

/// Same as LCD.DrawPixel will ignore out of bounds pixels (does not do modulus)
void draw_pixel_in_bounds(int x, int y);

/// Base class for a scene in the game
class Scene {
   public:
    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    virtual void update(double alpha);

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    virtual void physics_update(double t, double dt);
};

/// 2 dimensional mathmatical vector
struct Vector2 {
    float x = 0;
    float y = 0;

    Vector2();
    Vector2(float x, float y);

    Vector2& operator+=(const Vector2& b);
    Vector2& operator+=(float b);

    Vector2& operator-=(const Vector2& b);
    Vector2& operator-=(float b);

    Vector2& operator*=(const Vector2& b);
    Vector2& operator*=(float b);

    Vector2& operator/=(const Vector2& b);
    Vector2& operator/=(float b);

    Vector2 operator+(const Vector2& b) const;
    Vector2 operator+(float b) const;

    Vector2 operator-(const Vector2& b) const;
    Vector2 operator-(float b) const;

    Vector2 operator*(const Vector2& b) const;
    Vector2 operator*(float b) const;

    Vector2 operator/(const Vector2& b) const;
    Vector2 operator/(float b) const;

    static float dot(const Vector2& a, const Vector2& b);
    float dot(const Vector2& b) const;

    static float distance(const Vector2& a, const Vector2& b);
    float distance(const Vector2& b) const;

    float magnitude() const;
    Vector2 normalize() const;
};

/// Global varaible for what the current scene is
inline auto current_scene = std::make_shared<Scene>();