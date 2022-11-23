#pragma once

/// @file knife.h
/// Knife logic

#include <cstdint>

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

   private:
    /// Primitive for a point on the screen
    struct Point {
        int x;
        int y;
    };

    void draw_line(Point a, Point b);

    /// Ring buffer for storing the touch points to draw the knife between
    Point points[TAIL_LEN];

    /// Tail of the ring buffer
    uint64_t tail;

    /// Head of the ring buffer
    uint64_t head;
};