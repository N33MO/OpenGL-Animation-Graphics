//
//  Renderer.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 2/18/20.
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

#include "stb_image.h"
#include "Texture.h"

class Renderer
{
public:
    GLFWwindow* m_window;
    
    static Camera* m_camera;
    static Lighting* m_lightings;
    static nanogui::Screen* m_nanogui_screen;
    std::vector<Object> obj_list;
    
    bool is_scene_reset = true; // reset scene once in main frame while loop
    glm::vec4 background_color = glm::vec4( 0.1f , 0.1f , 0.1f , 0.1f ); // Main scene background color
    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;
    static bool keys[1024]; // get keboard input in this buffer
    
    // //
    nanogui::Color color_obj = Eigen::Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );
//    nanogui::Color color_dir_am = Eigen::Vector4f( 0.0f, 0.0f, 0.0f, 0.0f );
//    nanogui::Color color_dir_df = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//    nanogui::Color color_dir_sp = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//    nanogui::Color color_pnt_am = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//    nanogui::Color color_pnt_df = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//    nanogui::Color color_pnt_sp = Eigen::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 objCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    
public:
    Renderer();
    ~Renderer();
    
    void run();
    void init();
    void display(GLFWwindow *window);
    void nanogui_init(GLFWwindow *window);
    void scene_reset();
    void camera_move();
    void setup_uniform_values(Shader &shader);
    void draw_scene(Shader &shader);
    void draw_object(Shader &shader, Object &object);
    void load_models();
    void bind_vaovbo(Object &cur_obj);
    
    int rotate_value_in = 5;
    
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
    enum model_shade
    {
        shade1 = 0,
        shade2 = 1
    };
    enum model_depth
    {
        depth1 = 0,
        depth2 = 1
    };
    enum model_name
    {
        item1 = 0,
        item2 = 1,
        item3 = 2,
        item4 = 3
    };
    model_type enumType = type3;
    model_fface enumFace = face1;
    model_shade enumShade = shade2;
    model_depth enumDepth = depth1;
    model_name enumModel = item1;
    int model_select = enumModel;
    GLfloat obj_shininess = 32.0f;
    bool light_rotate_x = false;
    bool light_rotate_y = false;
    bool light_rotate_z = false;
    bool textureOn = false;
    bool normalOn = false;
};
