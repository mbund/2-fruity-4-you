/// @file throwable.cpp
/// Implementation of objects with physics

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>

#include "FEHLCD.h"
#include "FEHUtility.h"

#include "game.h"
#include "image.h"
#include "menu.h"
#include "throwable.h"
#include "ui.h"
#include "util.h"

#define PI 3.14159265358979323846

// https://www.jeffreythompson.org/collision-detection/line-circle.php

// POINT/CIRCLE
bool collide_point_circle(Vector2 p, Vector2 c, float r) {
    float distance = Vector2::distance(p, c);
    return distance <= r;
}

// LINE/POINT
bool collide_line_point(Vector2 l1, Vector2 l2, Vector2 p) {
    // get distance from the point to the two ends of the line
    float d1 = Vector2::distance(p, l1);
    float d2 = Vector2::distance(p, l2);

    // get the length of the line
    float line_length = Vector2::distance(l1, l2);

    // since floats are so minutely accurate, add
    // a little buffer zone that will give collision
    float buffer = 0.1f;  // higher # = less accurate

    // if the two distances are equal to the line's
    // length, the point is on the line!
    // note we use the buffer here to give a range,
    // rather than one #
    return d1 + d2 >= line_length - buffer && d1 + d2 <= line_length + buffer;
}

// LINE/CIRCLE
bool collide_line_circle(Vector2 l1, Vector2 l2, Vector2 c, float r) {
    // is either end INSIDE the circle?
    // if so, return true immediately
    bool inside_1 = collide_point_circle(l1, c, r);
    if (inside_1)
        return true;

    bool inside_2 = collide_point_circle(l2, c, r);
    if (inside_2)
        return true;

    // get the length of the line
    float line_length = Vector2::distance(l1, l2);

    // get dot product of the line and circle
    float dot =
        (((c.x - l1.x) * (l2.x - l1.x)) + ((c.y - l1.y) * (l2.y - l1.y))) /
        (line_length * line_length);

    // find the closest point on the line
    float closest_x = l1.x + (dot * (l2.x - l1.x));
    float closest_y = l1.y + (dot * (l2.y - l1.y));

    // is this point actually on the line segment?
    // if so keep going, but if not, return false
    bool on_segment = collide_line_point(l1, l2, {closest_x, closest_y});
    if (!on_segment)
        return false;

    // get distance to closest point
    float distance = Vector2::distance({closest_x, closest_y}, c);

    return distance <= r;
}

PhysicsObject::PhysicsObject(Vector2 pos, float mass)
    : prev_position(pos), current_position(pos), position(pos), mass(mass) {}

void PhysicsObject::update(double alpha) {
    position = current_position * alpha + prev_position * (1.0 - alpha);
    velocity = current_velocity * alpha + prev_velocity * (1.0 - alpha);
    // position = current_position;
    // velocity = current_velocity;
}

void PhysicsObject::physics_update(double t, double dt) {
    prev_position = current_position;
    prev_velocity = current_velocity;

    current_velocity += acceleration * dt;
    current_position += current_velocity * dt;

    // clear the acceleration every frame
    acceleration = {0, 0};
}

void PhysicsObject::add_force(const Vector2& force) {
    // Newton's 2nd law: f = m * a or a = f / m
    acceleration += force / mass;
}

Fruit::Fruit(std::string image_name, float radius, Vector2 pos, float mass)
    : PhysicsObject(pos, mass),
      should_be_removed(false),
      radius(radius),
      image_name(image_name),
      image(image_repository->load_image("assets/" + image_name + ".png")) {}

void Fruit::update(double alpha) {
    if (game->paused)
        return;

    PhysicsObject::update(alpha);

    if (position.y - radius > LCD_HEIGHT + 100) {
        should_be_removed = true;
    }

    image->render(position.x, position.y,
                  game->t * PI * std::clamp(velocity.x / 10.0f, -2.0f, 2.0f));
}

bool Fruit::get_should_be_removed() const {
    return should_be_removed;
}

void Fruit::physics_update(double t, double dt) {
    // apply gravity
    add_force({0, 3500.0f});

    PhysicsObject::physics_update(t, dt);
}

void Fruit::collision(Vector2 p1, Vector2 p2) {
    if (!should_be_removed && collide_line_circle(p1, p2, position, radius)) {
        should_be_removed = true;

        Vector2 force_left = {rand_range(-60000, -120000),
                              rand_range(-120000, 120000)};

        auto shard_left =
            std::make_unique<FruitShard>("assets/" + image_name + "-left.png",
                                         radius, position, force_left, mass);
        game->fruit_shards.push_back(std::move(shard_left));

        Vector2 force_right = {-force_left.x, -force_left.y};
        auto shard_right =
            std::make_unique<FruitShard>("assets/" + image_name + "-right.png",
                                         radius, position, force_right, mass);
        game->fruit_shards.push_back(std::move(shard_right));

        game->points += game->multiplier * std::log2(game->combo + 2);
        game->combo++;
        game->comboTime = TimeNow();
    }
}

Apple::Apple(Vector2 pos) : Fruit("apple", 13, pos, 8) {}
Bananas::Bananas(Vector2 pos) : Fruit("bananas", 13, pos, 8) {}
Orange::Orange(Vector2 pos) : Fruit("orange", 13, pos, 8) {}
Cherries::Cherries(Vector2 pos) : Fruit("cherries", 13, pos, 8) {}
Strawberry::Strawberry(Vector2 pos) : Fruit("strawberry", 13, pos, 8) {}
Pineapple::Pineapple(Vector2 pos) : Fruit("pineapple", 13, pos, 8) {}

Bomb::Bomb(Vector2 pos) : Fruit("bomb", 13, pos, 8) {}

void Bomb::update(double alpha) {
    if (game->paused)
        return;

    Fruit::update(alpha);

    LCD.SetFontColor(RED);
    draw_circle(position.x, position.y, radius + 5);
}

void Bomb::collision(Vector2 p1, Vector2 p2) {
    if (collide_line_circle(p1, p2, position, radius)) {
        // flags pausing
        game->paused = true;
        game->time_paused = TimeNow();

        LCD.SetFontColor(INDIANRED);
        LCD.FillCircle(position.x, position.y, 10);

        // explosion
        for (int i = 3; i < 100; i += 2) {
            LCD.SetFontColor(DARKGOLDENROD);
            fill_circle(position.x + rand_range(-4 - i, 4 + i),
                        position.y + rand_range(-4 - i, 4 + i),
                        rand_range(1, i));
            LCD.SetFontColor(RED);
            fill_circle(position.x + rand_range(-4 - i, 4 + i),
                        position.y + rand_range(-4 - i, 4 + i),
                        rand_range(1, i));
            LCD.SetFontColor(GRAY);
            fill_circle(position.x + rand_range(-4 - i, 4 + i),
                        position.y + rand_range(-4 - i, 4 + i),
                        rand_range(1, i));
            LCD.SetFontColor(FIREBRICK);
            fill_circle(position.x + rand_range(-4 - i, 4 + i),
                        position.y + rand_range(-4 - i, 4 + i),
                        rand_range(1, i));
            Sleep(2.0 / 100.0);
        }

        // screen wipe
        LCD.SetFontColor(BLACK);
        for (uint64_t i = 0; i < LCD_HEIGHT; i += 2) {
            LCD.DrawHorizontalLine(i, 0, LCD_WIDTH);
            LCD.DrawHorizontalLine(i + 1, 0, LCD_WIDTH);
            LCD.Update();
        }
    }
}

FruitShard::FruitShard(std::string image_path,
                       float radius,
                       Vector2 pos,
                       Vector2 force,
                       float mass)
    : PhysicsObject(pos, mass),
      should_be_removed(false),
      radius(radius),
      image(image_repository->load_image(image_path)) {
    add_force(force);
}

void FruitShard::update(double alpha) {
    PhysicsObject::update(alpha);

    if (position.y - radius > LCD_HEIGHT + 100) {
        should_be_removed = true;
    }

    image->render(position.x, position.y, game->t * PI);
}

bool FruitShard::get_should_be_removed() const {
    return should_be_removed;
}

void FruitShard::physics_update(double t, double dt) {
    // apply gravity
    add_force({0, 3500.0f});

    PhysicsObject::physics_update(t, dt);
}
