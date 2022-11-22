#pragma once

/// @file main.h
/// Global variables and definitions

// Use c++17 inline for global variables

#include <memory>

/// Global variable for whether the screen is currently being touched
inline bool touchPressed;
/// Global variable for the current touched x coordinate
inline int touchX;
/// Global variable for the current touched y coordinate
inline int touchY;

/// Base class for a scene in the game
class Scene {
   public:
    /// Update and render the scene
    /// @param dt delta time, the time elapsed since the start of the frame
    virtual void update(float dt);
};

/// Global varaible for what the current scene is
inline auto current_scene = std::make_shared<Scene>();