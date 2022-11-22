#include "Knife.h"
#include <FEHFile.h>
#include <FEHLCD.h>
#include <FEHSD.h>
#include <FEHUtility.h>
#include <random>

/// @brief Its a knife
Knife::Knife() {
    for (int i = 0; i < TAIL_LEN; i++) {
        x_points[i] = NO_X;
    }
}
/// @brief manages the disp and values of the knife
/// @param x - float x position of mouse
/// @param y - float y position of mouse
/// @param pressed checks to see if the mouse is pressed
void Knife ::UpdateK(float x, float y, bool pressed) {
    if (pressed) {
        /*converts to ints*/
        x_points[0] = (int)x;
        y_points[0] = (int)y;

        for (int i = 1; i < TAIL_LEN; i++) {
            /*should be drawing lines :/*/
            if (x_points[i] != NO_X) {
                LCD.DrawLine(x_points[i - 1], y_points[i - 1], x_points[i],
                             y_points[i]);
                /*LCD.SetFontColor(RED);
                LCD.DrawCircle(x_points[i],y_points[i],2);



                LCD.SetFontColor(ORANGE);
                LCD.DrawCircle((x_points[i]+7*x_points[i-1])>>3         ,
                (y_points[i]+7*y_points[i-1])>>3 ,2);

                LCD.SetFontColor(YELLOW);
                LCD.DrawCircle((x_points[i]+3*x_points[i-1])>>2         ,
                (y_points[i]+3*y_points[i-1])>>2 ,2);

                LCD.SetFontColor(GREEN);
                LCD.DrawCircle((x_points[i]+x_points[i-1])>>1         ,
                (y_points[i]+y_points[i-1])>>1 ,2);

                LCD.SetFontColor(BLUE);
                LCD.DrawCircle((3*x_points[i]+x_points[i-1])>>2         ,
                (3*y_points[i]+y_points[i-1])>>2 ,2);



                LCD.SetFontColor(VIOLET);
                LCD.DrawCircle((7*x_points[i]+x_points[i-1])>>3         ,
                (7*y_points[i]+y_points[i-1])>>3 ,2);

                     */
            } else
                break;
        }
        
        // moves all the values down
        for (int i = TAIL_LEN - 1; i > 0; i--) {
            x_points[i] = x_points[i - 1];
            y_points[i] = y_points[i - 1];
        }

        LCD.SetFontColor(RED);
        LCD.FillCircle(x_points[0], y_points[0], 4);

    } else
    //signifies a none continuous 
        x_points[1] = NO_X;
}
