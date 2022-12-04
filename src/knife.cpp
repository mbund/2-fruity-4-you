/// @file knife.cpp
/// @author John Ulm
/// @brief Knife implementation

#include <algorithm>
#include <cmath>

#include <FEHLCD.h>

#include "game.h"
#include "knife.h"
#include "util.h"

Knife::Knife() {}

// Bresenham's line drawing algorithm which works for all points a and b
void Knife::rainbow_draw_line(Point a, Point b) {
    int x, y;
    int xe, ye;

    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int dx1 = std::abs(dx);
    int dy1 = std::abs(dy);

    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;

    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = a.x;
            y = a.y;
            xe = b.x;
        } else {
            x = b.x;
            y = b.y;
            xe = a.x;
        }

        rainbow_dot(x, y);

        for (; x < xe; x++) {
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                    y++;
                else
                    y--;
                px += 2 * (dy1 - dx1);
            }

            rainbow_dot(x, y);
        }
    } else {
        if (dy >= 0) {
            x = a.x;
            y = a.y;
            ye = b.y;
        } else {
            x = b.x;
            y = b.y;
            ye = a.y;
        }

        rainbow_dot(x, y);

        for (; y < ye; y++) {
            if (py <= 0) {
                py = py + 2 * dx1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                    x++;
                else
                    x--;
                py += 2 * (dx1 - dy1);
            }

            rainbow_dot(x, y);
        }
    }
}

// Change font color to next one in a ranbow and makes cross
void Knife::rainbow_dot(int x, int y) {
    LCD.SetFontColor(colors[current_color]);
    current_color++;
    if (current_color == 7) {
        current_color = 0;
    }
    draw_pixel_in_bounds(x, y);
    draw_pixel_in_bounds(x + 1, y);
    draw_pixel_in_bounds(x - 1, y);
    draw_pixel_in_bounds(x, y + 1);
    draw_pixel_in_bounds(x, y - 1);
}

void Knife::update() {
    if (touchPressed) {
        points[head % TAIL_LEN] = {touchX, touchY};

        for (size_t i = tail; i < head; i++) {
            Point p1 = points[i % TAIL_LEN];
            Point p2 = points[(i + 1) % TAIL_LEN];

            current_color = 0;
            rainbow_draw_line(p1, p2);
        }

        if (head - tail > 1) {
            auto p1 = points[(head - 0) % TAIL_LEN];
            auto p2 = points[(head - 1) % TAIL_LEN];

            game->collide_with_knife(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y));
        }

        if (head < tail + TAIL_LEN - 1) {
            head++;
        } else {
            tail++;
        }

        LCD.SetFontColor(colors[0]);
        fill_circle(touchX, touchY, 3);

    } else {
        tail = head;
    }
}
