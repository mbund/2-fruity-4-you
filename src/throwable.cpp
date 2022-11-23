#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include "FEHLCD.h"
#include "FEHUtility.h"

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

Apple::Apple(Vector2 pos, float mass) : position(pos), mass(mass) {
    image = ImageRepository::load_image("assets/apple.png");
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

void Apple::update(float dt) {
    // apply gravity
    add_force({0, 9.81f * dt});

    velocity += acceleration;
    position += velocity;

    // clear the acceleration every frame
    acceleration = {0, 0};

    image->render(position.x, position.y, TimeNow() * M_PI);

    LCD.SetFontColor(color);
    draw_circle(position.x, position.y, mass);
}

void Apple::collision(Vector2 p1, Vector2 p2) {
    if (collide_line_circle(p1, p2, position, mass)) {
        color = FEHLCD::FEHLCDColor::Blue;
        add_force({rand_range(-2, 2), rand_range(-4, -2)});
    }
}

void Apple::add_force(Vector2 force) {
    // Newton's 2nd law: f = m * a
    // or a = f / m
    auto f = force / mass;
    acceleration += f;
}

void Apple::cut() {}

// Bomb::Bomb() {}
void Bomb::update(float dt) {}
void Bomb::cut() {}