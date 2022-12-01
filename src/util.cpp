/// @file util.cpp
/// Implementation of miscellaneous utilities

#include <cmath>

#include "FEHLCD.h"
#include "FEHRandom.h"

#include "ui.h"
#include "util.h"

void Scene::update(double alpha) {}
void Scene::physics_update(double t, double dt) {}

float rand_range(float lower, float upper) {
    float range = upper - lower;
    float randNum = (rand() % 1025) / 1024.0;

    return randNum * (range) + lower;
}

void draw_pixel_in_bounds(int x, int y) {
    if (x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT)
        LCD.DrawPixel(x, y);
}

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2& Vector2::operator+=(const Vector2& b) {
    x += b.x;
    y += b.y;
    return *this;
}

Vector2& Vector2::operator+=(float b) {
    x += b;
    y += b;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& b) {
    x -= b.x;
    y -= b.y;
    return *this;
}

Vector2& Vector2::operator-=(float b) {
    x -= b;
    y -= b;
    return *this;
}

Vector2& Vector2::operator*=(const Vector2& b) {
    x *= b.x;
    y *= b.y;
    return *this;
}

Vector2& Vector2::operator*=(float b) {
    x *= b;
    y *= b;
    return *this;
}

Vector2& Vector2::operator/=(const Vector2& b) {
    x /= b.x;
    y /= b.y;
    return *this;
}

Vector2& Vector2::operator/=(float b) {
    x /= b;
    y /= b;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& b) const {
    Vector2 copy(*this);
    copy += b;
    return copy;
}

Vector2 Vector2::operator+(float b) const {
    Vector2 copy(*this);
    copy += b;
    return copy;
}

Vector2 Vector2::operator-(const Vector2& b) const {
    Vector2 copy(*this);
    copy -= b;
    return copy;
}

Vector2 Vector2::operator-(float b) const {
    Vector2 copy(*this);
    copy -= b;
    return copy;
}

Vector2 Vector2::operator*(const Vector2& b) const {
    Vector2 copy(*this);
    copy *= b;
    return copy;
}

Vector2 Vector2::operator*(float b) const {
    Vector2 copy(*this);
    copy *= b;
    return copy;
}

Vector2 Vector2::operator/(const Vector2& b) const {
    Vector2 copy(*this);
    copy /= b;
    return copy;
}

Vector2 Vector2::operator/(float b) const {
    Vector2 copy(*this);
    copy /= b;
    return copy;
}

float Vector2::dot(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

float Vector2::dot(const Vector2& b) const {
    return dot(*this, b);
}

float Vector2::distance(const Vector2& a, const Vector2& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float Vector2::distance(const Vector2& b) const {
    return distance(*this, b);
}

float Vector2::magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalize() const {
    Vector2 copy(*this);
    copy /= copy.magnitude();
    return copy;
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

void draw_horizontal_line(int y, int x1, int x2) {
    if (x2 < x1) {
        int c = x2;
        x2 = x1;
        x1 = c;
    }
    for (int i = x1; i <= x2; i++) {
        draw_pixel_in_bounds(i, y);
    }
}

void draw_vertical_line(int x, int y1, int y2) {
    if (y2 < y1) {
        int c = y2;
        y2 = y1;
        y1 = c;
    }
    for (int i = y1; i <= y2; i++) {
        draw_pixel_in_bounds(x, i);
    }
}

void fill_circle(int x0, int y0, int r) {
    // This algorithm is a variant on DrawCircle.
    // Rather than draw the points around the circle,
    // We connect them with a series of lines
    // to fill in the circle.

    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    draw_vertical_line(x0, y0 - r, y0 + r);
    draw_horizontal_line(y0, x0 - r, x0 + r);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        draw_horizontal_line(y0 + x, x0 - y, x0 + y);
        draw_horizontal_line(y0 - x, x0 - y, x0 + y);
        draw_vertical_line(x0 + x, y0 - y, y0 + y);
        draw_vertical_line(x0 - x, y0 - y, y0 + y);
    }
}