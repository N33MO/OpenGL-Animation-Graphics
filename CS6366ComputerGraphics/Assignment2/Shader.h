//
//  Shader.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 2/18/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class Shader
{
public:
    GLuint program;
    Shader(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path, const GLchar* geometry_shader_path = nullptr)
    
    {
        std::string vertex_code;
        std::string fragment_code;
        std::string geometry_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;
        std::ifstream g_shader_file;
        
        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        try{
            v_shader_file.open(vertex_shader_path);
            std::stringstream v_shader_stream;
            v_shader_stream << v_shader_file.rdbuf();
            v_shader_file.close();
            vertex_code = v_shader_stream.str();
            
            f_shader_file.open(fragment_shader_path);
            std::stringstream f_shader_stream;
            f_shader_stream << f_shader_file.rdbuf();
            f_shader_file.close();
            fragment_code = f_shader_stream.str();
            
            if( geometry_shader_path != nullptr )
            {
                g_shader_file.open(geometry_shader_path);
                std::stringstream g_shader_stream;
                g_shader_stream << g_shader_file.rdbuf();
                g_shader_file.close();
                geometry_code = g_shader_stream.str();
            }
        } // try
        catch(const std::exception&)
        {
            std::cout << "Error: Shader not read.\n" << std::endl;
        }
        
        GLuint vertex, fragment, geometry;
        
        const char* v_shader_code = vertex_code.c_str();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);
        check_compile_error(vertex, "VERTEX");
        
        const char* f_shader_code = fragment_code.c_str();
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);
        check_compile_error(fragment, "FRAGMENT");
        
        if( geometry_shader_path != nullptr )
        {
            const char* g_shader_code = geometry_code.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &g_shader_code, NULL);
            glCompileShader(geometry);
            check_compile_error(geometry, "GEOMETRY");
        }
        
        this->program = glCreateProgram();
        glAttachShader(this->program, vertex);
        glAttachShader(this->program, fragment);
        if( geometry_shader_path != nullptr )
            glAttachShader(this->program, geometry);
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if( geometry_shader_path != nullptr )
            glDeleteShader(geometry);
        
        glLinkProgram(this->program);
        check_compile_error(this->program, "PROGRAM");
    }
    
    void use()
    {
        glUseProgram(this->program);
    }
    
private:
    void check_compile_error(GLuint shader, std::string type)
    {
        GLint success;
        GLchar info_log[1024];
        
        if( type == "PROGRAM" )
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if( !success )
            {
                glGetShaderInfoLog(shader, 1024, NULL, info_log);
                std::cout << "| Error:: PROGRAM_LINKING_ERROR of type: " << type << "|\n" << info_log << "\n| -- --------------------------------------------------- -- |\n";
            }
        }
        else if( type == "VERTEX" || type == "FRAGMENT" || type == "GEOMETRY" )
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if( !success )
            {
                glGetShaderInfoLog(shader, 1024, NULL, info_log);
                std::cout << "| Error:: SHADER_COMPILATION_ERROR of type: " << type << "|\n" << info_log << "\n| -- --------------------------------------------------- -- |\n";
            }
        }
        else
        {
            std::cout << "Error:: INCORRECT INPUT TYPE.\n";
        }
    }
};
