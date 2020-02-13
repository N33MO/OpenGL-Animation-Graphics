//
//  main.cpp -> tutorialQUAD.cpp
//  environmentsetup
//
//  Created by Apple on 2019/8/24.
//  Copyright © 2019 NEEMO. All rights reserved.
//

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// init useful variables
const GLuint width = 800, height = 600;



int main( void )
{
    GLFWwindow *window;
    
    // init the Library
    if (!glfwInit())
    {
        return -1;
    }
    glfwInit();
    
    // create a windowed mode and its OpenGL context
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // make the window's context current
    glfwMakeContextCurrent( window );
    
    float vertices[] =
    {
        0,300,0.0, //
        300,300,0.0, //
        300,0,0.0, //
        0,0,0.0 //

    };
    
    glViewport(0.0f, 0.0f, width, height); // specifies the part of the window to which openGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrixs defines the properties of the camera that views the objects in the world co0rdinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes.
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such a glOrtho and glRotate cumulate, basically puts us at (0, 0, 0).
    glOrtho(0, width, 0, height, 0, 1); // essentially set coordinate system.
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world.
    glLoadIdentity(); // same as above comment.
    
    // loop until the user close the window
    while (!glfwWindowShouldClose( window ))
    {
        glClear( GL_COLOR_BUFFER_BIT );
        
        // render the OpenGL, put all your OpenGL right here
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays( GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // swap front and back buffers
        glfwSwapBuffers( window );
        
        // poll for and process events
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
