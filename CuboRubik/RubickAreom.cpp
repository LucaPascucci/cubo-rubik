//#include <iostream>
//#include <cstdlib>
//
//
//#if defined(__APPLE__) || defined(MACOSX)
//#   include <GLUT/glut.h>
//#else
//#   include <GL/glut.h>
//#endif
//
//
//#include "rubix_cube.h"
//
//
//double rotate_y = 0; 
//double rotate_x = 0;
//
//int rotate_c = -1;
//int rotate_r = -1;
//int rotate_l = -1;
//
//GLfloat rotate_angle = 0.0 ;
//
//RubixCube *main_cube;
//
//void display();
//
//
//void init() 
//{
//    glEnable(GL_DEPTH_TEST);
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//    glClearColor(192/255.f, 192/255.f, 192/255.f, 1.0);
//}
//
//
//void init_cube(){
//    main_cube = new RubixCube;
//}
//
//
//// Arrow keys to rotate our view.
//void specialKeys( int key, int x, int y ) {
//    switch(key){
//        case GLUT_KEY_RIGHT:
//            rotate_y += 5;
//            break;
//        case GLUT_KEY_LEFT:
//            rotate_y -= 5;
//            break;
//        case GLUT_KEY_UP:
//            rotate_x += 5;
//            break;
//        case GLUT_KEY_DOWN:
//            rotate_x -= 5;
//            break;
//    }
// 
//    glutPostRedisplay();
//}
//
//
//// make the rotation effect when rotating a slice
//void rotate_animation(GLfloat step){
//    for(int i = 0; i < 90/step * 100; i+=1){
//        if(i % 100 == 0){
//            rotate_angle += step;
//            display();
//        }
//    }
//    rotate_angle = 0.0;
//}
//
//
//void reshape(int w, int h){
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45.0, w/(double)h, 1.0, 200);
//    gluLookAt(0.0,5.5, 15.0,
//              0.0,0.0,0.0,
//              0.0,1.0,0.0);
//}
//
//
//void keyboard(unsigned char key, int x, int y){
//    switch(key){
//        case 27:
//            exit(0);
//        case 'q':
//            rotate_c = 0; 
//            rotate_animation(3.0);
//            main_cube->rotate_column(0);
//            break;
//        case 'w':
//            rotate_c = 1; 
//            rotate_animation(3.0);
//            main_cube->rotate_column(1);
//            break;
//        case 'e':
//            rotate_c = 2; 
//            rotate_animation(3.0);
//            main_cube->rotate_column(2);
//            break;
//
//        case 'a':
//            rotate_r = 0;
//            rotate_animation(3.0);
//            main_cube->rotate_row(0);
//            break;
//        case 's':
//            rotate_r = 1;
//            rotate_animation(3.0);
//            main_cube->rotate_row(1);
//            break;
//        case 'd':
//            rotate_r = 2;
//            rotate_animation(3.0);
//            main_cube->rotate_row(2);
//            break;
//        
//        case 'z':
//            rotate_l = 0;
//            rotate_animation(3.0);
//            main_cube->rotate_level(0);
//            break;
//        case 'x':
//            rotate_l = 1;
//            rotate_animation(3.0);
//            main_cube->rotate_level(1);
//            break;
//        case 'c':
//            rotate_l = 2;
//            rotate_animation(3.0);
//            main_cube->rotate_level(2);
//            break;
//    }
//    glutPostRedisplay();
//    rotate_c = -1;
//    rotate_r = -1;
//    rotate_l = -1;
//}
//
//
//void display(){
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glTranslatef(-3.1, -3.1, -3.1);
//
//    glTranslatef(0.0, 3.1, 3.1);
//    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
//    glTranslatef(0.0, -3.1, -3.1);
//    glTranslatef(3.1, 0.0, 3.1);
//    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
//    glTranslatef(-3.1, 0.0, -3.1);
//
//    for(GLint x = 0; x < 3; x += 1){
//        for(GLint y = 0; y < 3; y += 1){
//            for(GLint z = 0; z < 3; z += 1){
//                glPushMatrix();
//
//                // We don't "really" rotate the cubes.
//                // Instead, we rotate a slice, and immediately reposition all small cubes after 
//                // switching their colors. 
//
//                glTranslatef(3.1, 3.1, 3.1);
//                if(x < 1){
//                    if(rotate_c == 0) glRotatef(-rotate_angle, 1.0, 0.0, 0.0);
//                }else if(x < 2){
//                    if(rotate_c == 1) glRotatef(-rotate_angle, 1.0, 0.0, 0.0);
//                }else if(x < 3){
//                    if(rotate_c == 2) glRotatef(-rotate_angle, 1.0, 0.0, 0.0);
//                }
//                
//                if(y < 1){
//                    if(rotate_r == 0) glRotatef(rotate_angle, 0.0, 1.0, 0.0);
//                }else if(y < 2){
//                    if(rotate_r == 1) glRotatef(rotate_angle, 0.0, 1.0, 0.0);
//                }else if(y < 3){
//                    if(rotate_r == 2) glRotatef(rotate_angle, 0.0, 1.0, 0.0);
//                }
//                
//                if(z < 1){
//                    if(rotate_l == 0) glRotatef(-rotate_angle, 0.0, 0.0, 1.0);
//                }else if(z < 2){
//                    if(rotate_l == 1) glRotatef(-rotate_angle, 0.0, 0.0, 1.0);
//                }else if(z < 3){
//                    if(rotate_l == 2) glRotatef(-rotate_angle, 0.0, 0.0, 1.0);
//                }
//                glTranslatef(-3.1, -3.1, -3.1);
//
//                glTranslatef(x * 2.1, y * 2.1, z * 2.1);
//                
//                main_cube->display(x, y, z);
//                
//                glPopMatrix();
//            }
//        }
//    }
//    glutSwapBuffers();
//}
//    
//
//int main(int argc, char *argv[]){
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(800, 800);
//    glutCreateWindow("Rubik's Cube");
//    //glutFullScreen();
//
//    init();
//    init_cube();
//
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutKeyboardFunc(keyboard);
//    glutSpecialFunc(specialKeys);
//
//    glutMainLoop();
//    
//    delete main_cube;
//    return 0;
//}