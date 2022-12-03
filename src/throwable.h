#pragma once

/// @file throwable.h
/// Throwable objects with physics

#include <memory>

#include "image.h"
#include "util.h"

/// Base class for objects which have physics
class PhysicsObject {
   public:
    /// Create a physics object at a given position with a given mass
    /// @param pos screenspace position coordinate to place the object
    /// @param mass mass of the physics object
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
    void add_force(const Vector2& force);

   protected:
    Vector2 prev_position, current_position, position;
    Vector2 prev_velocity, current_velocity, velocity;
    Vector2 acceleration;
    float mass;
};

/// Throwable fruit
class Fruit : public PhysicsObject {
   public:
    /// Create a fruit object
    /// @param image_name path to image to display as the base
    /// @param radius collision radius in pixels
    /// @param pos screenspace position coordinate to place the object
    /// @param mass mass of the underlying physics object
    Fruit(std::string image_name, float radius, Vector2 pos, float mass);

    /// Update and render the fruit
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha) override;

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    void physics_update(double t, double dt) override;

    /// Detect collision between the apple and a line
    /// @param p1 first point on line
    /// @param p2 second point on line
    virtual void collision(Vector2 p1, Vector2 p2);

    /// Getter for if the fruit should be destroyed
    virtual bool get_should_be_removed() const;

   protected:
    bool should_be_removed;
    float radius;
    std::string image_name;
    std::shared_ptr<Image> image;
};

/// Throwable apple fruit
class Apple final : public Fruit {
   public:
    /// Create an apple
    /// @param pos screenspace position coordinate to place the apple
    Apple(Vector2 pos);
};

/// Throwable bananas
class Bananas final : public Fruit {
   public:
    /// Create a banana
    /// @param pos screenspace position coordinate to place the banana
    Bananas(Vector2 pos);
};

/// Throwable orange
class Orange final : public Fruit {
   public:
    /// Create an orange
    /// @param pos screenspace position coordinate to place the orange
    Orange(Vector2 pos);
};

/// Throwable cherries
class Cherries final : public Fruit {
   public:
    /// Create a cherry
    /// @param pos screenspace position coordinate to place the cherry
    Cherries(Vector2 pos);
};

/// Throwable strawberry
class Strawberry final : public Fruit {
   public:
    /// Create a strawberry
    /// @param pos screenspace position coordinate to place the strawberry
    Strawberry(Vector2 pos);
};

/// Throwable pineapple
class Pineapple final : public Fruit {
   public:
    /// Create a pineapple
    /// @param pos screenspace position coordinate to place the pineapple
    Pineapple(Vector2 pos);
};

/// Throwable bomb
class Bomb final : public Fruit {
   public:
    /// Create a bomb
    /// @param pos screenspace position coordinate to place the bomb
    Bomb(Vector2 pos);

    /// Update and render the fruit shard
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    void update(double alpha) override;

    /// Detect collision between the bomb and a line
    /// @param p1 first point on line
    /// @param p2 second point on line
    void collision(Vector2 p1, Vector2 p2) override;
};

/// Shard of a fruit (one half after it is cut)
class FruitShard final : public PhysicsObject {
   public:
    /// Create a fruit shard
    /// @param image_name path to image to display as the base
    /// @param radius collision radius in pixels
    /// @param pos screenspace position coordinate to place the fruit shard
    /// @param mass mass of the underlying physics object
    FruitShard(std::string image_path,
               float radius,
               Vector2 pos,
               Vector2 force,
               float mass);

    /// Update and render the fruit shard
    /// @param alpha physics alpha, for interpolation between previous state and
    /// next state
    virtual void update(double alpha);

    /// Run physics calculations
    /// @param t time since start of game
    /// @param dt physics timestep
    virtual void physics_update(double t, double dt);

    /// Getter for if the fruit shard should be destroyed
    virtual bool get_should_be_removed() const;

   private:
    bool should_be_removed;
    float radius;
    std::shared_ptr<Image> image;
};
