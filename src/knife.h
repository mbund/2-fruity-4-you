#pragma once

/// @file knife.h
/// Knife logic

#include <cstdint>

#include "FEHLCD.h"
#include "LCDColors.h"
#include "util.h"

/// Length of the tail
#define TAIL_LEN 5

/// Representation of the knife, moving with the mouse
class Knife {
   public:
    /// Default constructor
    Knife();

    /// Update and render the knife
    void update();

    /// Primitive for a point on the screen
    struct Point {
        int x;
        int y;
    };

   private:
    void draw_line(Point a, Point b);

    /// Ring buffer for storing the touch points to draw the knife between
    Point points[TAIL_LEN];

    /// Tail of the ring buffer
    uint64_t tail;

    /// Head of the ring buffer
    uint64_t head;

    /// colors
    /// red, orange, yellow, green, blue, violet, indigo
    unsigned int colors[7] = {0xCD5C5C, 0xFFA500, 0xFFFF00, 0x008000,
                              0x0000FF, 0x4B0082, 0xEE82EE};
    int current_color;
};