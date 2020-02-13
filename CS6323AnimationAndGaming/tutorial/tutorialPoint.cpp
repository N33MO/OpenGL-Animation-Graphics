//
//  tutorialPoint.cpp
//  environmentsetup
//
//  Created by Apple on 2019/8/26.
//  Copyright © 2019 NEEMO. All rights reserved.
//

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 640;

int main( void )
{
    GLFWwindow *window;
    
    // initialize the library
    if ( !glfwInit() ) {
        return -1;
    }
    
    // create a windowed mode and its OpenGL Context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Draw Point", NULL, NULL);
    
    if ( !window ) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    
    // make the window's context here
    glfwMakeContextCurrent( window );
    
    glViewport(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT); // specifies the part of the window to which openGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrixs defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes.
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such a glOrtho and glRotate cumulate, basically puts us at (0, 0, 0).
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0, 1); // essentially set coordinate system.
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world.
    glLoadIdentity(); // same as above comment.
    
    GLfloat pointVertex1[] = { WINDOW_WIDTH * 0.5 , WINDOW_HEIGHT * 0.5 };
    GLfloat pointVertex2[] = { WINDOW_WIDTH * 0.75 , WINDOW_HEIGHT * 0.5 };
    
    // loop until the user close the window
    while ( !glfwWindowShouldClose( window ))
    {
        glClear( GL_COLOR_BUFFER_BIT );
        
        // render the OpenGL, put all your OpenGL right here
        
        // seems like glEnable( GL_POINT_SMOOTH ) is conflict with glPointSize( GLfloat size ). 9/4/2019
        
        glEnable( GL_POINT_SMOOTH );
        glEnableClientState( GL_VERTEX_ARRAY );
        glPointSize( 100.0 );
        glVertexPointer(2, GL_FLOAT, 0, pointVertex1);
        glDrawArrays(GL_POINTS, 0, 1);
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisable( GL_POINT_SMOOTH );
        
//        glBegin( GL_POINT_SIZE );
//        glEnable( GL_POINT_SMOOTH );
        glEnableClientState( GL_VERTEX_ARRAY );
        glPointSize( 200.0 );
        glVertexPointer(2, GL_FLOAT, 0, pointVertex2);
        glDrawArrays(GL_POINTS, 0, 1);
        glDisableClientState( GL_VERTEX_ARRAY );
//        glDisable( GL_POINT_SMOOTH );
//        glEnd();
        
        // swap front and back buffers
        glfwSwapBuffers( window );
        
        // poll for and process events
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
