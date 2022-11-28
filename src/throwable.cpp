/// @file throwable.cpp
/// Implementation of objects with physics

#include <memory>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

#include "FEHLCD.h"
#include "FEHUtility.h"

#include "game.h"
#include "image.h"
#include "throwable.h"
#include "ui.h"
#include "util.h"

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
    bool inside_2 = collide_point_circle(l2, c, r);
    if (inside_1 || inside_2)
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

void draw_circle(int x0, int y0, int r) {
    // This alogorithm is from wikipedia
    // It's called the "midpoint circle algorithm"
    // or the "Bresenham's circle algorithm"
    // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    // See the page for further details
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    draw_pixel_in_bounds(x0, y0 + r);
    draw_pixel_in_bounds(x0, y0 - r);
    draw_pixel_in_bounds(x0 + r, y0);
    draw_pixel_in_bounds(x0 - r, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        draw_pixel_in_bounds(x0 + x, y0 + y);
        draw_pixel_in_bounds(x0 - x, y0 + y);
        draw_pixel_in_bounds(x0 + x, y0 - y);
        draw_pixel_in_bounds(x0 - x, y0 - y);
        draw_pixel_in_bounds(x0 + y, y0 + x);
        draw_pixel_in_bounds(x0 - y, y0 + x);
        draw_pixel_in_bounds(x0 + y, y0 - x);
        draw_pixel_in_bounds(x0 - y, y0 - x);
    }
}

PhysicsObject::PhysicsObject(Vector2 pos, float mass)
    : prev_position(pos),
      position(pos),
      prev_velocity({0, 0}),
      velocity({0, 0}),
      mass(mass) {}

void PhysicsObject::update(double alpha) {
    position = position * alpha + prev_position * (1.0 - alpha);
    velocity = velocity * alpha + prev_velocity * (1.0 - alpha);
}
void PhysicsObject::physics_update(double t, double dt) {
    prev_position = position;
    prev_velocity = velocity;

    velocity += acceleration * dt;
    position += velocity * dt;

    // clear the acceleration every frame
    acceleration = {0, 0};
}

void PhysicsObject::add_force(const Vector2& force) {
    // Newton's 2nd law: f = m * a or a = f / m
    acceleration += force / mass;
}

Apple::Apple(Vector2 pos, float mass)
    : PhysicsObject(pos, mass),
      should_be_removed(false),
      image(ImageRepository::load_image("assets/apple.png")) {}

void Apple::update(double alpha) {
    PhysicsObject::update(alpha);

    if (position.y - mass > LCD_HEIGHT + 100) {
        should_be_removed = true;
    }

    image->render(
        position.x, position.y,
        TimeNow() * M_PI * std::clamp(velocity.x / 10.0f, -10.0f, 10.0f));
}

bool Apple::get_should_be_removed() {
    return should_be_removed;
}

void Apple::physics_update(double t, double dt) {
    // apply gravity
    add_force({0, 3500.0f});

    PhysicsObject::physics_update(t, dt);
}

void Apple::collision(Vector2 p1, Vector2 p2) {
    if (!should_be_removed && collide_line_circle(p1, p2, position, mass)) {
        should_be_removed = true;
        Vector2 force_left = {rand_range(-100000, -40000),
                              rand_range(160000, -160000)};
        auto shard_left = std::make_unique<AppleShard>(
            "assets/apple-left.png", mass, position, force_left);
        game->apple_shards.push_back(std::move(shard_left));

        Vector2 force_right = {-force_left.x, -force_left.y};
        auto shard_right = std::make_unique<AppleShard>(
            "assets/apple-right.png", mass, position, force_right);
        game->apple_shards.push_back(std::move(shard_right));
    }
}

AppleShard::AppleShard(std::string image_path,
                       float mass,
                       Vector2 pos,
                       Vector2 force)
    : PhysicsObject(pos, mass),
      should_be_removed(false),
      image(ImageRepository::load_image(image_path)) {
    add_force(force);
}

void AppleShard::update(double alpha) {
    PhysicsObject::update(alpha);

    if (position.y - mass > LCD_HEIGHT + 100) {
        should_be_removed = true;
    }

    image->render(position.x, position.y, TimeNow() * M_PI);
}

bool AppleShard::get_should_be_removed() {
    return should_be_removed;
}

void AppleShard::physics_update(double t, double dt) {
    // apply gravity
    add_force({0, 3500.0f});

    PhysicsObject::physics_update(t, dt);
}
