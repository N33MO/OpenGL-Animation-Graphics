//
//  Renderer.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 1/31/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#pragma once

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <nanogui/nanogui.h>
#include <nanogui/checkbox.h>

#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "Lighting.h"

class Renderer
{
public:
    GLFWwindow* m_window;
    
    static Camera* m_camera;
    
    static Lighting* m_lightings;
    
    static nanogui::Screen* m_nanogui_screen;
    
    std::vector<Object> obj_list;
    
    glm::vec4 background_color = glm::vec4( 0.1f, 0.1f, 0.1f, 0.1f );
    
    bool is_scene_reset = true;
    
//    std::string model_name;
    
    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;
    
    static bool keys[1024];
    
public:
    Renderer();
    ~Renderer();
    
    void nanogui_init(GLFWwindow* window);
    void init();
    
    void display(GLFWwindow* window);
    void run();
    
    void camera_move();
    
    void load_models();
    
    void draw_scene(Shader& shader);
    void draw_object(Shader& shader, Object& object);
    
    void bind_vaovbo(Object &cur_obj);
    
    void setup_uniform_values(Shader& shader);
    void scene_reset();
    
    // //
    int rotate_value_in = 5;
    
    enum model_name
    {
        item1 = 0,
        item2 = 1,
        item3 = 2
    };
    enum model_type
    {
        type1 = 0,
        type2 = 1,
        type3 = 2
    };
    enum model_fface
    {
        face1 = 0,
        face2 = 1
    };
    
    model_name enumval = item1;
    model_type enumType = type2;
    model_fface enumFace = face1;
    int model_select = enumval;
};

