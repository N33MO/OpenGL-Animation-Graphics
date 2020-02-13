//
//  tutorialColor.cpp
//  OpenGLProjects
//
//  Created by Dayuan Chen on 9/6/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// set Window Size
const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 640;

int main( void )
{
    GLFWwindow *window;
    
    // initialize the library
    if ( !glfwInit() ) {
        return -1;
    }
    
    // create a windowed mode and its OpenGL Context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Color", NULL, NULL);
    
    if ( !window ) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    
    // make the window's context here
    glfwMakeContextCurrent( window );
    
    glViewport(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels.
    glMatrixMode( GL_PROJECTION ); // projection matrixs defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes.
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such a glOrtho and glRotate cumulate, basically puts us at (0, 0, 0).
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0, 1); // essentially set coordinate system.
    glMatrixMode( GL_MODELVIEW ); // (defult matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world.
    glLoadIdentity(); // same as above comment.
    
    GLfloat halfScreenWidth = WINDOW_WIDTH / 2;
    GLfloat halfScreenHeight = WINDOW_HEIGHT / 2;
    
    GLfloat halfSideLength = 200;
    
    GLfloat vertices[] =
    {
        halfScreenWidth, halfScreenHeight + halfSideLength, 0.0, // top center vertex
        halfScreenWidth - halfSideLength, halfScreenHeight - halfSideLength, 0.0, // bottom left corner
        halfScreenWidth + halfSideLength, halfScreenHeight - halfSideLength, 0.0 // bottom right corner
    };
    
    GLfloat color[] =
    {
        255, 0, 0, // R
        0, 255, 0, // G
        0, 0, 255 // B
    };
    
    // loop until the user close the window
    while ( !glfwWindowShouldClose( window )) {
        glClear( GL_COLOR_BUFFER_BIT );
        
        // put the OpenGL code here
        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_COLOR_ARRAY );
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glColorPointer(3, GL_FLOAT, 0, color);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableClientState( GL_COLOR_ARRAY );
        glDisableClientState( GL_VERTEX_ARRAY );
        
        // swap front and back buffers
        glfwSwapBuffers( window );
        
        // poll for and process events
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
