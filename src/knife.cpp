/// @file knife.cpp
/// Knife implementation

#include <algorithm>
#include <cmath>

#include <FEHLCD.h>

#include "game.h"
#include "knife.h"

Knife::Knife() {}

/// Bresenham's line drawing algorithm which works for all points a and b
void Knife::draw_line(Point a, Point b) {
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

        LCD.DrawPixel(x, y);

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

            LCD.DrawPixel(x, y);
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

        LCD.DrawPixel(x, y);

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

            LCD.DrawPixel(x, y);
        }
    }
}

template <typename T>
void fruit_collision(Knife::Point p1, Knife::Point p2, T& a) {
    std::for_each(a.begin(), a.end(), [p1, p2](auto& b) {
        b->collision({(float)p1.x, (float)p1.y}, {(float)p2.x, (float)p2.y});
    });
}

void Knife::update() {
    if (touchPressed) {
        LCD.SetFontColor(GREEN);
        LCD.FillCircle(touchX, touchY, 2);

        if (head < tail + TAIL_LEN) {
            points[head % TAIL_LEN] = {touchX, touchY};

            for (size_t i = tail + 1; i < head; i++) {
                Point p1 = points[i % TAIL_LEN];
                Point p2 = points[(i + 1) % TAIL_LEN];

                draw_line(p1, p2);
            }

            if (head - tail > 1) {
                auto p1 = points[(head - 0) % TAIL_LEN];
                auto p2 = points[(head - 1) % TAIL_LEN];
                fruit_collision(p1, p2, game->apples);
                fruit_collision(p1, p2, game->bananas);
            }

            head++;
        } else {
            tail++;
        }

    } else {
        tail = head;
    }
}
