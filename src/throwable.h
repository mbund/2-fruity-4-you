#pragma once

/// @file throwable.h
/// Throwable objects with physics

#include <memory>
#include <vector>

#include "FEHLCD.h"

#include "image.h"
#include "util.h"

/// Base class for objects which have physics
class PhysicsObject {
   public:
    /// Default constructor
    PhysicsObject(Vector2 pos, float mass);

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    virtual void update(double alpha);

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    virtual void physics_update(double t, double dt);

    /// Add an impulse force to the object
    /// @param force 2D force vector to apply as an impulse force
    virtual void add_force(const Vector2& force);

   protected:
    Vector2 prev_position, position;
    Vector2 prev_velocity, velocity;
    Vector2 acceleration;
    float mass;
};

/// Throwable apple fruit
class Apple : public PhysicsObject {
   public:
    /// Default constructor
    Apple(Vector2 pos, float mass);

    /// Update and render the apple
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha);

    /// Detect collision between the apple and a line
    /// @param p1 first point on line
    /// @param p2 second point on line
    void collision(Vector2 p1, Vector2 p2);

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    void physics_update(double t, double dt);

    /// Getter for if the apple should be destroyed
    bool get_should_be_removed();

   private:
    bool should_be_removed;
    std::shared_ptr<Image> image;
};

/// Shard of an apple (one half after it is cut)
class AppleShard : public PhysicsObject {
   public:
    /// Default constructor
    AppleShard(std::string image_path, float mass, Vector2 pos, Vector2 force);

    /// Update and render the apple shard
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha);

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    void physics_update(double t, double dt);

    /// Getter for if the apple shard should be destroyed
    bool get_should_be_removed();

   private:
    bool should_be_removed;
    std::shared_ptr<Image> image;
};