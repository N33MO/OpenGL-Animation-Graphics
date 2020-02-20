//
//  Renderer.cpp
//  OpenGLProjects
//
//  Created by Dayuan Chen on 1/31/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#include "Renderer.h"

Camera* Renderer::m_camera = new Camera();

Lighting* Renderer::m_lightings = new Lighting();

nanogui::Screen* Renderer::m_nanogui_screen = nullptr;

bool Renderer::keys[1024];

nanogui::Color colval(1.0f, 1.0f, 0.0f, 1.0f);

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::nanogui_init(GLFWwindow *window)
{
    m_nanogui_screen = new nanogui::Screen();
    m_nanogui_screen->initialize(window, true);
    
    glViewport(0, 0, m_camera->width, m_camera->height);
    
    // glfwSwapInterval(0);
    // glfwSwapBuffers(window);
    
    // Create nanogui gui
    nanogui::FormHelper *gui_1 = new nanogui::FormHelper(m_nanogui_screen);
    nanogui::ref<nanogui::Window> nanoguiWindow_1 = gui_1->addWindow(Eigen::Vector2i(10,10), "Nanogui Control Bar_1");
    
    // screen->setPosition(Eigen::Vector2i(-width/2 + 200, -height/2 + 300));
    
    // // nanogui bar configurations
    gui_1->addGroup("Color");
    gui_1->addVariable("Color", colval);
    
    gui_1->addGroup("Position");
    gui_1->addVariable("X", m_camera->position.x)->setSpinnable(true);
    gui_1->addVariable("Y", m_camera->position.y)->setSpinnable(true);
    gui_1->addVariable("Z", m_camera->position.z)->setSpinnable(true);
    
    gui_1->addGroup("Rotate");
    gui_1->addVariable("Rotate Value", rotate_value_in)->setSpinnable(true);
    gui_1->addButton("Rotate X axis +", [this]() { m_camera->rotate_x(rotate_value_in); });
    gui_1->addButton("Rotate X axis -", [this]() { m_camera->rotate_x(-rotate_value_in); });
    gui_1->addButton("Rotate Y axis +", [this]() { m_camera->rotate_y(rotate_value_in); });
    gui_1->addButton("Rotate Y axis -", [this]() { m_camera->rotate_y(-rotate_value_in); });
    gui_1->addButton("Rotate Z axis +", [this]() { m_camera->rotate_z(rotate_value_in); });
    gui_1->addButton("Rotate Z axis -", [this]() { m_camera->rotate_z(-rotate_value_in); });
    
    gui_1->addGroup("Configuration");
    gui_1->addVariable("Z near",m_camera->near)->setSpinnable(true);
    gui_1->addVariable("Z far",m_camera->far)->setSpinnable(true);
    gui_1->addVariable("zoom",m_camera->zoom)->setSpinnable(true);
    gui_1->addVariable("Render Type", enumType, true)->setItems({ "Point", "Line", "Triangles" });
    gui_1->addVariable("Culling Type", enumFace, true)->setItems({ "CW", "CCW" });
    gui_1->addVariable("Model Name", enumval, true)->setItems({ "cube", "cyborg", "rock" });
    gui_1->addButton("Reset Model", [this]() {
        // //
        model_select = enumval;
        m_camera->gui_btn_reset();
        obj_list[model_select].calculate_center();
        m_camera->objCenter = glm::vec3(obj_list[model_select].obj_center.x,obj_list[model_select].obj_center.y,obj_list[model_select].obj_center.z);
        m_camera->front = m_camera->objCenter - m_camera->position;
    });
    gui_1->addButton("Reset Camera", [this]() {
        scene_reset();
    });
    
//    gui_1->addButton("Reload Model", []() {/* reload model function */});
//    gui_1->addButton("Reset", []() {/* reset function */});
    
    m_nanogui_screen->setVisible(true);
    m_nanogui_screen->performLayout();
    
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
        m_nanogui_screen->cursorPosCallbackEvent(x, y);
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow *, int button, int action, int modifiers) {
        m_nanogui_screen->mouseButtonCallbackEvent(button, action, modifiers);
    });
    
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        // screen->keyCallbackEvent(key, scancode, action, mods);
        
        if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if( key >= 0 && key < 1024 )
        {
            if( action == GLFW_PRESS )
            {
                keys[key] = true;
            }
            else if( action == GLFW_RELEASE )
            {
                keys[key] = false;
            }
        }
    });
    
    glfwSetCharCallback(window, [](GLFWwindow *, unsigned int codepoint) {
        m_nanogui_screen->charCallbackEvent(codepoint);
    });
    
    glfwSetDropCallback(window, [](GLFWwindow *, int count, const char **filenames) {
        m_nanogui_screen->dropCallbackEvent(count, filenames);
    });
    
    glfwSetScrollCallback(window, [](GLFWwindow *, double x, double y) {
        m_nanogui_screen->scrollCallbackEvent(x, y);
        // m_camera->ProcessMouseScroll(y);
    });
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int width, int height) {
        m_nanogui_screen->resizeCallbackEvent(width, height);
    });
}

void Renderer::init()
{
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    m_camera->init();
    m_lightings->init();
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    this->m_window = glfwCreateWindow(m_camera->width, m_camera->height, "Assignment 0", nullptr, nullptr);
    glfwMakeContextCurrent(this->m_window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    nanogui_init(this->m_window);
}

void Renderer::display(GLFWwindow* window)
{
    Shader m_shader = Shader("./shader/basic.vert", "./shader/basic.frag");
    
    // Main frame while loop
    while( !glfwWindowShouldClose(window) )
    {
        glfwPollEvents();
        
        if( is_scene_reset )
        {
            scene_reset();
            is_scene_reset = false;
        }
        
        camera_move();
        
        m_shader.use();
        
        setup_uniform_values(m_shader);
        
        load_models();
        draw_scene(m_shader);
        
        m_nanogui_screen->drawWidgets();
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return;
}

void Renderer::run()
{
    init();
    display(this->m_window);
}

void Renderer::load_models()
{
    obj_list.clear();
    Object cube_object("./models/cube.obj");
    cube_object.obj_color = glm::vec4(colval[0], colval[1], colval[2], colval[3]);
    cube_object.obj_name = "cube";
    
    Object cyborg_object("./models/cyborg.obj");
    cyborg_object.obj_color = glm::vec4(colval[0], colval[1], colval[2], colval[3]);
    cyborg_object.obj_name = "cyborg";
    
    Object rock_object("./models/rock.obj");
    rock_object.obj_color = glm::vec4(colval[0], colval[1], colval[2], colval[3]);
    rock_object.obj_name = "rock";
    
    switch (enumType) {
        case 0:
            cube_object.m_render_type = RENDER_POINTS;
            cyborg_object.m_render_type = RENDER_POINTS;
            rock_object.m_render_type = RENDER_POINTS;
            break;
        case 1:
            cube_object.m_render_type = RENDER_LINES;
            cyborg_object.m_render_type = RENDER_LINES;
            rock_object.m_render_type = RENDER_LINES;
            break;
        case 2:
            cube_object.m_render_type = RENDER_TRIANGLES;
            cyborg_object.m_render_type = RENDER_TRIANGLES;
            rock_object.m_render_type = RENDER_TRIANGLES;
            break;
            
        default:
            break;
    }
    
    bind_vaovbo(cube_object);
    bind_vaovbo(cyborg_object);
    bind_vaovbo(rock_object);
    
    obj_list.push_back(cube_object);
    obj_list.push_back(cyborg_object);
    obj_list.push_back(rock_object);
}

void Renderer::draw_scene(Shader& shader)
{
    // Set up some basic parameters
    glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    switch (enumFace) {
        case 0:
            glFrontFace(GL_CW);
            break;
        case 1:
            glFrontFace(GL_CCW);
            
        default:
            break;
    }
    
    for(size_t i = 0; i < obj_list.size(); i++)
    {
        if( obj_list[i].obj_name == "cube" && model_select == 0 )
        {
            // Draw cube
            glm::mat4 cur_cube_model_mat = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(cur_cube_model_mat));
            draw_object(shader, obj_list[i]);
        }
        if( obj_list[i].obj_name == "cyborg" && model_select == 1 )
        {
            // Draw cyborg
            glm::mat4 cur_cyborg_model_mat = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(cur_cyborg_model_mat));
            draw_object(shader, obj_list[i]);
        }
        if( obj_list[i].obj_name == "rock" && model_select == 2 )
        {
            // Draw rock
            glm::mat4 cur_rock_model_mat = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(cur_rock_model_mat));
            draw_object(shader, obj_list[i]);
        }
    }
}

void Renderer::camera_move()
{
    GLfloat current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    // Camera controls
    if( keys[GLFW_KEY_W] )
        m_camera->process_keyboard(FORWARD, delta_time);
    if( keys[GLFW_KEY_A] )
        m_camera->process_keyboard(LEFT, delta_time);
    if( keys[GLFW_KEY_S] )
        m_camera->process_keyboard(BACKWARD, delta_time);
    if( keys[GLFW_KEY_D] )
        m_camera->process_keyboard(RIGHT, delta_time);
    if( keys[GLFW_KEY_Q] )
        m_camera->process_keyboard(UP, delta_time);
    if( keys[GLFW_KEY_E] )
        m_camera->process_keyboard(DOWN, delta_time);
    if( keys[GLFW_KEY_I] )
        m_camera->process_keyboard(ROTATE_X_UP, delta_time);
    if( keys[GLFW_KEY_J] )
        m_camera->process_keyboard(ROTATE_Y_UP, delta_time);
    if( keys[GLFW_KEY_K] )
        m_camera->process_keyboard(ROTATE_X_DOWN, delta_time);
    if( keys[GLFW_KEY_L] )
        m_camera->process_keyboard(ROTATE_Y_DOWN, delta_time);
    if( keys[GLFW_KEY_U] )
        m_camera->process_keyboard(ROTATE_Z_UP, delta_time);
    if( keys[GLFW_KEY_O] )
        m_camera->process_keyboard(ROTATE_Z_DOWN, delta_time);
}

void Renderer::draw_object(Shader &shader, Object &object)
{
    glBindVertexArray(object.vao);
    
    glUniform3f(glGetUniformLocation(shader.program, "m_object.object_color"), object.obj_color[0], object.obj_color[1], object.obj_color[2]);
    glUniform1f(glGetUniformLocation(shader.program, "m_object.shininess"),  object.shininess);
    
    if( object.m_render_type == RENDER_TRIANGLES )
    {
        if( object.m_obj_type == OBJ_POINTS )
        {
            std::cout << "Error: Cannot render triangles if input obj type is point.\n";
            return;
        }
        if( object.m_obj_type == OBJ_TRIANGLES )
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, object.vao_vertices.size());
        }
    }
    
    if( object.m_render_type == RENDER_LINES )
    {
        glLineWidth(20.0f);
        if( object.m_obj_type == OBJ_POINTS )
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINE_LOOP, 0, object.vao_vertices.size());
        }
        if( object.m_obj_type == OBJ_TRIANGLES )
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, object.vao_vertices.size());
        }
    }
    if( object.m_render_type == OBJ_POINTS )
    {
        glPointSize(1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
        glDrawArrays(GL_POINTS, 0, object.vao_vertices.size());
    }
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::bind_vaovbo(Object &cur_obj)
{
    glGenVertexArrays(1, &cur_obj.vao);
    glGenBuffers(1, &cur_obj.vbo);
    
    glBindVertexArray(cur_obj.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, cur_obj.vbo);
    glBufferData(GL_ARRAY_BUFFER, cur_obj.vao_vertices.size() * sizeof(Object::Vertex), &cur_obj.vao_vertices[0], GL_STATIC_DRAW);
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, TexCoords));
    
    glBindVertexArray(0);
}

void Renderer::setup_uniform_values(Shader &shader)
{
    // Camera uniform values
    glUniform3f(glGetUniformLocation(shader.program, "camera_pos"), m_camera->position.x, m_camera->position.y, m_camera->position.z);
    
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(m_camera->get_projection_mat()));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(m_camera->get_view_mat())); // // no change
    
    // Light uniform values
    glUniform1i(glGetUniformLocation(shader.program, "dir_light.status"), m_lightings->direction_light.status);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.direction"), m_lightings->direction_light.direction[0], m_lightings->direction_light.direction[1], m_lightings->direction_light.direction[2]);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.ambient"), m_lightings->direction_light.ambient[0], m_lightings->direction_light.ambient[1], m_lightings->direction_light.ambient[2]);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.diffuse"), m_lightings->direction_light.diffuse[0], m_lightings->direction_light.diffuse[1], m_lightings->direction_light.diffuse[2]);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.specular"), m_lightings->direction_light.specular[0], m_lightings->direction_light.specular[1], m_lightings->direction_light.specular[2]);
    
    // Set current point light as camera's position
    m_lightings->point_light.position = m_camera->position;
    glUniform1i(glGetUniformLocation(shader.program, "point_light.status"), m_lightings->point_light.status);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.position"), m_lightings->point_light.position[0], m_lightings->point_light.position[1], m_lightings->point_light.position[2]);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.ambient"), m_lightings->point_light.ambient[0], m_lightings->point_light.ambient[1], m_lightings->point_light.ambient[2]);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.diffuse"), m_lightings->point_light.diffuse[0], m_lightings->point_light.diffuse[1], m_lightings->point_light.diffuse[2]);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.specular"), m_lightings->point_light.specular[0], m_lightings->point_light.specular[1], m_lightings->point_light.specular[2]);
    glUniform1f(glGetUniformLocation(shader.program, "point_light.constant"), m_lightings->point_light.constant);
    glUniform1f(glGetUniformLocation(shader.program, "point_light.linear"), m_lightings->point_light.linear);
    glUniform1f(glGetUniformLocation(shader.program, "point_light.quadratic"), m_lightings->point_light.quadratic);
}

void Renderer::scene_reset()
{
    m_camera->reset();
}
