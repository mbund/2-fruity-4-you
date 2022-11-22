#include <FEHFile.h>
#include <FEHLCD.h>
#include <FEHSD.h>
#include <FEHUtility.h>
#include <random>

///length of the tail
#define TAIL_LEN 4
///arbitarily high number
#define NO_X 100000

class Knife {
   private:
    int x_points[TAIL_LEN], y_points[TAIL_LEN];
    bool pressed = false;

   public:
    Knife();
    void UpdateK(float x, float y, bool pressed);
};