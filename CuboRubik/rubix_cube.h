#ifndef RUBIX_CUBE_H
#define RUBIX_CUBE_H

#include <cstdlib>

#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif



typedef enum _Color{
    RED = 0,    //FRONT
    ORANGE,     //BACK
    GREEN,      //RIGHT
    BLUE,       //LEFT
    YELLOW,     //TOP
    WHITE       //BOTTOM
}Color;


// Each small cube of the big cube is a separate instance.
class SinglePiece{
    public:
        SinglePiece();
        Color color[6];
        void column_switch();
        void row_switch();
        void level_switch();
};

// The 3x3 Rubik's Cube
class RubixCube{
    public:
        RubixCube();
        void rotate_column(size_t n);
        void rotate_row(size_t n);
        void rotate_level(size_t n);
        void display(size_t x, size_t y, size_t z); // display a specific single cube
    
    private:
        SinglePiece cubes[3][3][3];
        void display_color(Color c); // set the colors before drawing a cube
};

#endif