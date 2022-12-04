#pragma once

/// @file knife.h
/// @author John Ulm
/// @brief Knife logic

#include <cstddef>
#include <cstdint>

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

    /// Draw a rainbow line
    /// @param a first endpoint of line to draw the rainbow line in screenspace
    /// @param b last endpoint of line to draw the rainbow line in screenspace
    void rainbow_draw_line(Point a, Point b);

    /// Draw a colored dot in the rainbow cycle
    /// @param x screenspace x coordinate of where to draw the dot
    /// @param y screenspace y coordinate of where to draw the dot
    void rainbow_dot(int x, int y);

    /// Length of the tail
    static const size_t TAIL_LEN = 4;

    /// Ring buffer for storing the touch points to draw the knife between
    Point points[TAIL_LEN];

    /// Tail of the ring buffer
    uint64_t tail;

    /// Head of the ring buffer
    uint64_t head;

    /// Rainbow color cycle
    /// Red, orange, yellow, green, blue, violet, indigo
    unsigned int colors[7] = {
        0xFF0000,  // red
        0xFFA500,  // orange
        0xFFFF00,  // yellow
        0x008000,  // green
        0x0000FF,  // blue,
        0x4B0082,  // indigo
        0xEE82EE,  // violet
    };

    /// Current index into the color cycle
    size_t current_color;
};