#pragma once

/// @file util.h
/// Global variables and definitions

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

inline float rand_range(float lower, float upper) {
    std::random_device rd;   // obtain a random number from hardware
    std::mt19937 gen(rd());  // seed the generator
    std::uniform_real_distribution<> distr(lower, upper);  // define the range

    return distr(gen);
}

/// Base class for a scene in the game
class Scene {
   public:
    /// Update and render the scene
    /// @param dt delta time, the time elapsed since the start of the frame
    virtual void update(float dt);
};

/// 2 dimensional mathmatical vector
struct Vector2 {
    float x;
    float y;

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
};

/// Global varaible for what the current scene is
inline auto current_scene = std::make_shared<Scene>();