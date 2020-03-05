//
//  Texture.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 3/2/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class TextureLoading
{
public:
    static GLuint LoadTexture( std::string path )
    {
        GLuint textureID;
        
        //====================
        // Texture
        //====================
        glGenTextures( 1, &textureID );
        glBindTexture( GL_TEXTURE_2D, textureID );
        // Set our texture parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        // Set texture filtering
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        // Load, create texture and generate mipmaps
        int width, height, nrChannels;
//        path = "/Users/n33mo/MyLib/XcodeProjects/OpenGLProjects/OpenGLProjects/CS6366ComputerGraphics/Assets/textures/cube_diffuse.png";
//        std::cout << "one proc" << std::endl;
//        std::cout << path << std::endl;
//        std::string path_cube_n = "/Users/n33mo/MyLib/XcodeProjects/OpenGLProjects/OpenGLProjects/CS6366ComputerGraphics/Assignment2/cube_normal.png";
//        std::string path_cube_d = "/Users/n33mo/MyLib/XcodeProjects/OpenGLProjects/OpenGLProjects/CS6366ComputerGraphics/Assignment2/cube_diffuse.png";
//        std::string path_cyborg_n = "/Users/n33mo/MyLib/XcodeProjects/OpenGLProjects/OpenGLProjects/CS6366ComputerGraphics/Assignment2/cyborg_normal.png";
//        std::string path_cyborg_d = "/Users/n33mo/MyLib/XcodeProjects/OpenGLProjects/OpenGLProjects/CS6366ComputerGraphics/Assignment2/cyborg_diffuse.png";
        unsigned char *image = stbi_load( path.c_str(), &width, &height, &nrChannels, STBI_rgb );
        if (!image) {
            std::cout<< "no image loaded." << std::endl;
        }
//        std::cout << "channels: " << nrChannels << " - p: " << path << std::endl;
//        if (path == path_cube_n) {
//            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
//        } else {
//            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
//            std::cout << "loading cube_normal." << std::endl;
//        }
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );
        stbi_image_free( image );
        glBindTexture( GL_TEXTURE_2D, 0 );
        
        return textureID;
    }
};
