//
//  tutorialTriangle.cpp
//  OpenGLProjects
//
//  Created by Dayuan Chen on 9/8/19.
//  Copyright © 2019 N33MO. All rights reserved.
//

// add "shader" file to Build Phases - Copy Files

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "shaderexample.h"

const GLint WIDTH = 800, HEIGHT = 640;

//const GLchar *vertexShaderSource =
//"#version 330 core\n"
//"layout ( location = 0 ) in vec3 position;\n"
//"void main( )\n"
//"{\n"
//"gl_Position = vec4( position.x, position.y, position.z, 1.0 );\n"
//"}\n";
//
//const GLchar *fragmentShaderSource =
//"#version 330 core\n"
//"out vec4 color;\n"
//"void main( )\n"
//"{\n"
//"color = vec4( 1.0f, 0.5f, 0.2f, 0.1f );\n"
//"}\n";

int main( )
{
    glfwInit();
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE ); // required for Mac users, won't conflict with Win
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", NULL, NULL);
    
    // following two lines required for Mac Users, won't conflict with Win
    //Get actual window size that relevant to your Mac (Retina Display things)
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( !window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    glewExperimental = GL_TRUE;
    
    if ( GLEW_OK != glewInit() )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        
        return -1;
    }
    
    glViewport(0, 0, screenWidth, screenHeight);
    
    // Shader
//    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
//    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
//    glCompileShader( vertexShader );
//
//    GLint success;
//    GLchar infoLog[512];
//
//    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
//
//    if ( !success ) {
//        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
//    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
//    glCompileShader( fragmentShader );
//
//    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
//
//    if ( !success ) {
//        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    GLuint shaderProgram = glCreateProgram( );
//    glAttachShader( shaderProgram, vertexShader );
//    glAttachShader( shaderProgram, fragmentShader );
//    glLinkProgram( shaderProgram );
//
//    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
//
//    if ( !success ) {
//        glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader( vertexShader );
//    glDeleteShader( fragmentShader );
    
    Shader ourShader("shader/basic.vert", "shader/basic.frag"); // /Users/n33mo/MyLib/XcodeProjects/OpenGLProjects/OpenGLProjects/tutorialModernOpenGL/
    
    GLfloat vertices[] =
    {
        // position        // color
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // middle top
    };
    
    GLuint VAO, VBO;
    
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * ) ( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 1 );
    
    glBindVertexArray( 0 );
    
    while ( !glfwWindowShouldClose( window ))
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT );
        
        ourShader.Use( );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        glBindVertexArray( 0 );
        
        glfwSwapBuffers( window );
    }
    
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    
    glfwTerminate();
    
    return 0;
}

