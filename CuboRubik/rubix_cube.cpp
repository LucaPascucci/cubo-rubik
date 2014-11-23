#include "rubix_cube.h"

// Initialize the colors
SinglePiece::SinglePiece(){
    color[0] = RED;         //FRONT
    color[1] = ORANGE;      //BACK
    color[2] = GREEN;       //RIGHT
    color[3] = BLUE;        //LEFT
    color[4] = YELLOW;      //TOP
    color[5] = WHITE;       //BOTTOM
}

// Define the behavior of a small cube whe rotating a column.
// x is constant
void SinglePiece::column_switch(){
    Color temp = color[0];
    color[0] = color[5];
    color[5] = color[1];
    color[1] = color[4];
    color[4] = temp;
}


// Define the behavior of a small cube whe rotating a row.
// y is constant
void SinglePiece::row_switch(){
    Color temp = color[0];
    color[0] = color[3];
    color[3] = color[1];
    color[1] = color[2];
    color[2] = temp;
}


// Define the behavior of a small cube whe rotating a level.
// z is constant
void SinglePiece::level_switch(){
    Color temp = color[4] ;
    color[4] = color[3];
    color[3] = color[5];
    color[5] = color[2];
    color[2] = temp;
}


// Initialize the 9 cubes
RubixCube::RubixCube(){
    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            for(int z = 0; z < 3; z++){
                SinglePiece p;
                cubes[x][y][z] = p;
            }
        }
    }
}


// Rotate a slice:
// 1.switch small cubes' colors
// 2.rotate the 3x3 cube matrix
void RubixCube::rotate_column(size_t n){
    // x == n
    SinglePiece temp = cubes[n][0][0];
    cubes[n][2][0].column_switch(); cubes[n][0][0] = cubes[n][2][0];
    cubes[n][2][2].column_switch(); cubes[n][2][0] = cubes[n][2][2];
    cubes[n][0][2].column_switch(); cubes[n][2][2] = cubes[n][0][2];
    temp.column_switch();           cubes[n][0][2] = temp;

    temp = cubes[n][0][1];
    cubes[n][1][0].column_switch(); cubes[n][0][1] = cubes[n][1][0];
    cubes[n][2][1].column_switch(); cubes[n][1][0] = cubes[n][2][1];
    cubes[n][1][2].column_switch(); cubes[n][2][1] = cubes[n][1][2];
    temp.column_switch();           cubes[n][1][2] = temp;

    cubes[n][1][1].column_switch();
}


void RubixCube::rotate_row(size_t n){
    // y == n
    SinglePiece temp = cubes[0][n][0];
    cubes[2][n][0].row_switch();    cubes[0][n][0] = cubes[2][n][0];
    cubes[2][n][2].row_switch();    cubes[2][n][0] = cubes[2][n][2];
    cubes[0][n][2].row_switch();    cubes[2][n][2] = cubes[0][n][2];
    temp.row_switch();              cubes[0][n][2] = temp;

    temp = cubes[0][n][1];
    cubes[1][n][0].row_switch();    cubes[0][n][1] = cubes[1][n][0];
    cubes[2][n][1].row_switch();    cubes[1][n][0] = cubes[2][n][1];
    cubes[1][n][2].row_switch();    cubes[2][n][1] = cubes[1][n][2];
    temp.row_switch();              cubes[1][n][2] = temp;

    cubes[1][n][1].row_switch();
}


void RubixCube::rotate_level(size_t n){
    // z == n
    SinglePiece temp = cubes[0][0][n];
    cubes[2][0][n].level_switch();  cubes[0][0][n] = cubes[2][0][n];
    cubes[2][2][n].level_switch();  cubes[2][0][n] = cubes[2][2][n];
    cubes[0][2][n].level_switch();  cubes[2][2][n] = cubes[0][2][n];
    temp.level_switch();            cubes[0][2][n] = temp;

    temp = cubes[0][1][n];
    cubes[1][0][n].level_switch();  cubes[0][1][n] = cubes[1][0][n];
    cubes[2][1][n].level_switch();  cubes[1][0][n] = cubes[2][1][n];
    cubes[1][2][n].level_switch();  cubes[2][1][n] = cubes[1][2][n];
    temp.level_switch();            cubes[1][2][n] = temp;

    cubes[1][1][n].level_switch();
}


void RubixCube::display(size_t x, size_t y, size_t z){
    SinglePiece p = cubes[x][y][z];
    // FRONT
    glBegin(GL_POLYGON);
        display_color(p.color[0]);
        glVertex3f( 2.0, 0.0, 2.0 );
        glVertex3f( 2.0, 2.0, 2.0 );
        glVertex3f( 0.0, 2.0, 2.0 );
        glVertex3f( 0.0, 0.0, 2.0 );
    glEnd();
        
    // BACK
    glBegin(GL_POLYGON);
        display_color(p.color[1]);
        glVertex3f( 2.0, 0.0, 0.0 );      
        glVertex3f( 2.0, 2.0, 0.0 );      
        glVertex3f( 0.0, 2.0, 0.0 );      
        glVertex3f( 0.0, 0.0, 0.0 );     
    glEnd();
 
    // RIGHT
    glBegin(GL_POLYGON);
        display_color(p.color[2]);
        glVertex3f( 2.0, 0.0, 0.0 );
        glVertex3f( 2.0, 2.0, 0.0 );
        glVertex3f( 2.0, 2.0, 2.0 );
        glVertex3f( 2.0, 0.0, 2.0 );
    glEnd();
 
    // LEFT
    glBegin(GL_POLYGON);
        display_color(p.color[3]);
        glVertex3f( 0.0, 0.0, 2.0 );
        glVertex3f( 0.0, 2.0, 2.0 );
        glVertex3f( 0.0, 2.0, 0.0 );
        glVertex3f( 0.0, 0.0, 0.0 );
    glEnd();
 
    // TOP
    glBegin(GL_POLYGON);
        display_color(p.color[4]);
        glVertex3f( 2.0, 2.0, 2.0 );
        glVertex3f( 2.0, 2.0, 0.0 );
        glVertex3f( 0.0, 2.0, 0.0 );
        glVertex3f( 0.0, 2.0, 2.0 );
    glEnd();
 
    // BOTTOM
    glBegin(GL_POLYGON);
        display_color(p.color[5]);
        glVertex3f( 2.0, 0.0, 0.0 );
        glVertex3f( 2.0, 0.0, 2.0 );
        glVertex3f( 0.0, 0.0, 2.0 );
        glVertex3f( 0.0, 0.0, 0.0 );
    glEnd();
}


void RubixCube::display_color(Color c){
    switch(c){
        case RED:
            glColor3ub( 255, 000, 000 );  // red
            break;
        case ORANGE:
            glColor3ub( 255, 165, 000 );  // orange
            break;
        case GREEN:
            glColor3ub( 000, 255, 000 );  // green
            break;
        case BLUE:
            glColor3ub( 000, 000, 255 );  // blue
            break;
        case YELLOW:
            glColor3ub( 255, 255, 000 );  // yellow
            break;
        case WHITE:
            glColor3ub( 255, 255, 255 );  // white
            break;
    }
}