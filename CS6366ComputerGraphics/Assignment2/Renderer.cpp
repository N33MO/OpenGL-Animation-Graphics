//
//  Renderer.cpp
//  OpenGLProjects
//
//  Created by Dayuan Chen on 2/18/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#include "Renderer.h"

Camera* Renderer::m_camera = new Camera();
Lighting* Renderer::m_lightings = new Lighting();
nanogui::Screen* Renderer::m_nanogui_screen = nullptr;

bool Renderer::keys[1024];

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::run()
{
    init();
    display(this->m_window);
}

void Renderer::init() // called by run()
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
    this->m_window = glfwCreateWindow(m_camera->width, m_camera->height, "Assignment1", nullptr, nullptr);
    glfwMakeContextCurrent(this->m_window);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    nanogui_init(this->m_window);
}

void Renderer::display(GLFWwindow *window) // called by run()
{
    Shader m_shader = Shader("./shader/basic.vert", "./shader/basic.frag");
    
    // Main frame while loop
    while( !glfwWindowShouldClose(window) )
    {
        // //
//        // check 'FlatOn'
//        if ( enumShade == 0 )
//            m_shader = Shader("./shader/basic_flat.vert", "./shader/basic_flat.frag");
//        else // enumShade == 1
//            m_shader = Shader("./shader/basic.vert", "./shader/basic.frag");
        
        glfwPollEvents();
        
        if( is_scene_reset == true ) { scene_reset(); is_scene_reset = false; }
        
        camera_move();
        
        m_shader.use();
        
        setup_uniform_values(m_shader);
        
        draw_scene(m_shader);
        
        m_nanogui_screen->drawWidgets();
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return;
}

void Renderer::nanogui_init(GLFWwindow *window) // called by init()
{
    m_nanogui_screen = new nanogui::Screen();
    m_nanogui_screen->initialize(window, true);
    
    glViewport(0, 0, m_camera->width, m_camera->height);
    
    // Create nanogui gui
    nanogui::FormHelper *gui_1 = new nanogui::FormHelper(m_nanogui_screen);
    nanogui::ref<nanogui::Window> nanoguiWindow_1 = gui_1->addWindow(Eigen::Vector2i(10,10), "Nanogui Control Bar_1");
    // nanogui bar configurations
    gui_1->addGroup("Position");
    static auto camera_x_widget = gui_1->addVariable("X", m_camera->position.x);
    static auto camera_y_widget = gui_1->addVariable("Y", m_camera->position.y);
    static auto camera_z_widget = gui_1->addVariable("Z", m_camera->position.z);
    camera_x_widget->setSpinnable(true);
    camera_y_widget->setSpinnable(true);
    camera_z_widget->setSpinnable(true);
    
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
    gui_1->addVariable("Shading Type", enumShade, true)->setItems({ "FLAT", "SMOOTH" });
    gui_1->addVariable("Depth Type", enumDepth, true)->setItems({ "LESS", "ALWAYS" });
    gui_1->addVariable("Model Name", enumModel, true)->setItems({ "cube", "cyborg", "rock", "two_cubes" });
    gui_1->addButton("Reset Model", [this]() {
        // //
        model_select = enumModel;
        obj_list[model_select].calculate_center();
        m_camera->gui_btn_reset();
        objCenter = glm::vec3(obj_list[model_select].obj_center.x,obj_list[model_select].obj_center.y,obj_list[model_select].obj_center.z);
        m_camera->front = objCenter - m_camera->position;

        load_models();
        
        camera_x_widget->setValue(m_camera->position[0]);
        camera_y_widget->setValue(m_camera->position[1]);
        camera_z_widget->setValue(m_camera->position[2]);
        
    });
    gui_1->addButton("Reset Camera", [this]() {
        scene_reset();
        camera_x_widget->setValue(m_camera->position[0]);
        camera_y_widget->setValue(m_camera->position[1]);
        camera_z_widget->setValue(m_camera->position[2]);
    });
    
    // Add another nanogui bar
    nanogui::ref<nanogui::Window> nanoguiWindow_2 = gui_1->addWindow(Eigen::Vector2i(220,10), "Nanogui Control Bar_2");
    gui_1->addGroup("Lighting");
    gui_1->addVariable("Object Color", color_obj);
    gui_1->addVariable("Object shininess", obj_shininess)->setSpinnable(true);
    gui_1->addVariable("Direction Light Status", m_lightings->direction_light.status);
    gui_1->addVariable("Direction Light Ambient Color", m_lightings->direction_light.ambient);
    gui_1->addVariable("Direction Light Diffuse Color", m_lightings->direction_light.diffuse);
    gui_1->addVariable("Direction Light Specular Color", m_lightings->direction_light.specular);
    gui_1->addVariable("Point Light Status", m_lightings->point_light.status);
    gui_1->addVariable("Point Light Ambient Color", m_lightings->point_light.ambient);
    gui_1->addVariable("Point Light Diffuse Color", m_lightings->point_light.diffuse);
    gui_1->addVariable("Point Light Specular Color", m_lightings->point_light.specular);
    gui_1->addVariable("Point Light rotate velocity", m_lightings->rotate_velocity);
    gui_1->addVariable("Point Light rotate on X", light_rotate_x);
    gui_1->addVariable("Point Light rotate on Y", light_rotate_y);
    gui_1->addVariable("Point Light rotate on Z", light_rotate_z);
    gui_1->addVariable("Texture status", textureOn);
    gui_1->addVariable("Normal map status", normalOn);
    gui_1->addButton("Reset Point Light", []() {
        m_lightings->point_light.position = m_camera->position;
    });
    
    //
    m_nanogui_screen->setVisible(true);
    m_nanogui_screen->performLayout();
    
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
        m_nanogui_screen->cursorPosCallbackEvent(x, y);
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow *, int button, int action, int modifiers) {
        m_nanogui_screen->mouseButtonCallbackEvent(button, action, modifiers);
    });
    
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if( key >= 0 && key < 1024 )
        {
            if( action == GLFW_PRESS )
                keys[key] = true;
            else if( action == GLFW_RELEASE )
                keys[key] = false;
        }
        // //
        camera_x_widget->setValue(m_camera->position[0]);
        camera_y_widget->setValue(m_camera->position[1]);
        camera_z_widget->setValue(m_camera->position[2]);
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

void Renderer::scene_reset() // called by display(window)
{
    load_models();
    m_camera->reset();
}

void Renderer::camera_move() // called by display(window)
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

void Renderer::setup_uniform_values(Shader &shader) // called by display(window)
{
    // Camera uniform values
    glUniform3f(glGetUniformLocation(shader.program, "camera_pos"), m_camera->position.x, m_camera->position.y, m_camera->position.z);
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(m_camera->get_projection_mat()));
     glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(m_camera->get_view_mat()));
    
    // Light uniform values - Direction Light
    glUniform1i(glGetUniformLocation(shader.program, "dir_light.status"), m_lightings->direction_light.status);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.direction"), m_lightings->direction_light.direction[0], m_lightings->direction_light.direction[1], m_lightings->direction_light.direction[2]);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.ambient"), m_lightings->direction_light.ambient[0], m_lightings->direction_light.ambient[1], m_lightings->direction_light.ambient[2]);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.diffuse"), m_lightings->direction_light.diffuse[0], m_lightings->direction_light.diffuse[1], m_lightings->direction_light.diffuse[2]);
    glUniform3f(glGetUniformLocation(shader.program, "dir_light.specular"), m_lightings->direction_light.specular[0], m_lightings->direction_light.specular[1], m_lightings->direction_light.specular[2]);
    
    // Light uniform values - Point Light
    glUniform1i(glGetUniformLocation(shader.program, "point_light.status"), m_lightings->point_light.status);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.position"), m_lightings->point_light.position[0], m_lightings->point_light.position[1], m_lightings->point_light.position[2]);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.ambient"), m_lightings->point_light.ambient[0], m_lightings->point_light.ambient[1], m_lightings->point_light.ambient[2]);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.diffuse"), m_lightings->point_light.diffuse[0], m_lightings->point_light.diffuse[1], m_lightings->point_light.diffuse[2]);
    glUniform3f(glGetUniformLocation(shader.program, "point_light.specular"), m_lightings->point_light.specular[0], m_lightings->point_light.specular[1], m_lightings->point_light.specular[2]);
    glUniform1f(glGetUniformLocation(shader.program, "point_light.constant"), m_lightings->point_light.constant);
    glUniform1f(glGetUniformLocation(shader.program, "point_light.linear"), m_lightings->point_light.linear);
    glUniform1f(glGetUniformLocation(shader.program, "point_light.quadratic"), m_lightings->point_light.quadratic);
//    glUniform1i( glGetUniformLocation( shader.program, "TexDiffuse" ), 0);
//    glUniform1i( glGetUniformLocation( shader.program, "TexNormal" ), 1);
    glUniform1i( glGetUniformLocation( shader.program, "FlatOn" ), (int)enumShade);
    
    // //
    // Set current point light position as camera's position
    //m_lightings->point_light.position = m_camera->position;
}

void Renderer::draw_scene(Shader &shader) // called by display(window)
{
    // Set up some basic parameters
    glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
//    glShadeModel(GL_SMOOTH); // use different shaders
    
    glEnable(GL_DEPTH_TEST);
    switch (enumDepth) {
        case 0:
            glDepthFunc(GL_LESS); break;
        case 1:
            glDepthFunc(GL_ALWAYS); break;
        default: break;
    }
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    switch (enumFace) {
        case 0:
            glFrontFace(GL_CW); break;
        case 1:
            glFrontFace(GL_CCW); break;
        default: break;
    }
    
    glUniform1i( glGetUniformLocation(shader.program, "DiffuseOn"), (int)textureOn);
    glUniform1i( glGetUniformLocation(shader.program, "NormalOn"), (int)normalOn);
    if (textureOn || normalOn) { // actually we enable differnet effect in shader.program
        std::string modelName = "";
        switch (model_select) {
            case 0:
                modelName = "cube";
                break;
            case 1:
                modelName = "cyborg";
                break;
            default:
                std::cout << "model have no texture file." << std::endl;
                break;
        }
        std::string texPath, normPath;
        texPath = modelName + "_diffuse.png";
        normPath = modelName + "_normal.png";
        std::string absolute = "dont know why i have to use absolute path";
        GLuint texID = TextureLoading::LoadTexture( absolute + texPath );
        GLuint normID = TextureLoading::LoadTexture( absolute + normPath );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texID );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, normID );
        glUniform1i( glGetUniformLocation( shader.program, "TexDiffuse" ), 0);
        glUniform1i( glGetUniformLocation( shader.program, "TexNormal" ), 1);
    }
    
    for(size_t i = 0; i < obj_list.size(); i++)
    {
        // //
        if (!m_lightings->direction_light.status && !m_lightings->point_light.status) {
            obj_list[i].obj_color = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
        }
        else{
            obj_list[i].obj_color = glm::vec4(color_obj[0], color_obj[1], color_obj[2], color_obj[3]);
        }
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
        if( obj_list[i].obj_name == "two_cubes" && model_select == 3 )
        {
            // Draw two cubes
            glm::mat4 cur_two_cubes_model_mat = glm::mat4(1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(cur_two_cubes_model_mat));
            draw_object(shader, obj_list[i]);
        }
    }
    
    if ( light_rotate_x && !light_rotate_y && !light_rotate_z )
        m_lightings->point_light_rotate(0, obj_list[model_select].obj_center);
    if ( !light_rotate_x && light_rotate_y && !light_rotate_z )
        m_lightings->point_light_rotate(1, obj_list[model_select].obj_center);
    if ( !light_rotate_x && !light_rotate_y && light_rotate_z )
        m_lightings->point_light_rotate(2, obj_list[model_select].obj_center);
}

void Renderer::draw_object(Shader &shader, Object &object) // called by draw_scene(shader)
{
    glBindVertexArray(object.vao);
    glUniform3f(glGetUniformLocation(shader.program, "m_object.object_color"), object.obj_color[0], object.obj_color[1], object.obj_color[2]);
    glUniform1f(glGetUniformLocation(shader.program, "m_object.shininess"),  obj_shininess); // // object.shininess -> obj_shininess
    
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
    
//    glUniform1i( glGetUniformLocation( shader.program, "ourTexture" ), 0 );
    
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::load_models() // called by scene_reset()
{
    obj_list.clear();
    Object cube_object("./models/cube.obj");
    cube_object.obj_color = glm::vec4(color_obj[0], color_obj[1], color_obj[2], color_obj[3]);
    cube_object.obj_name = "cube";
    
    Object cyborg_object("./models/cyborg.obj");
    cyborg_object.obj_color = glm::vec4(color_obj[0], color_obj[1], color_obj[2], color_obj[3]);
    cyborg_object.obj_name = "cyborg";
    
    Object rock_object("./models/rock.obj");
    rock_object.obj_color = glm::vec4(color_obj[0], color_obj[1], color_obj[2], color_obj[3]);
    rock_object.obj_name = "rock";
    
    Object two_cubes_object("./models/two_cubes.obj");
    two_cubes_object.obj_color = glm::vec4(color_obj[0], color_obj[1], color_obj[2], color_obj[3]);
    two_cubes_object.obj_name = "two_cubes";
    
    // // TODO
//    obj_list.push_back(cube_object);
//    obj_list.push_back(cyborg_object);
//    obj_list.push_back(rock_object);
//    obj_list.push_back(two_cubes_object);
//
//    Object target = obj_list[enumModel];
//
//    switch (enumType) {
//        case 0:
//            target.m_render_type = RENDER_POINTS; break;
//        case 1:
//            target.m_render_type = RENDER_LINES; break;
//        case 2:
//            target.m_render_type = RENDER_TRIANGLES; break;
//        default: break;
//    }
//    bind_vaovbo(target);
    
    switch (enumType) {
        case 0:
            cube_object.m_render_type = RENDER_POINTS;
            cyborg_object.m_render_type = RENDER_POINTS;
            rock_object.m_render_type = RENDER_POINTS;
            two_cubes_object.m_render_type = RENDER_POINTS;
            break;
        case 1:
            cube_object.m_render_type = RENDER_LINES;
            cyborg_object.m_render_type = RENDER_LINES;
            rock_object.m_render_type = RENDER_LINES;
            two_cubes_object.m_render_type = RENDER_LINES;
            break;
        case 2:
            cube_object.m_render_type = RENDER_TRIANGLES;
            cyborg_object.m_render_type = RENDER_TRIANGLES;
            rock_object.m_render_type = RENDER_TRIANGLES;
            two_cubes_object.m_render_type = RENDER_TRIANGLES;
            break;
            
        default:
            break;
    }
    
    bind_vaovbo(cube_object);
    bind_vaovbo(cyborg_object);
    bind_vaovbo(rock_object);
    bind_vaovbo(two_cubes_object);
    
    obj_list.push_back(cube_object);
    obj_list.push_back(cyborg_object);
    obj_list.push_back(rock_object);
    obj_list.push_back(two_cubes_object);
    
}

void Renderer::bind_vaovbo(Object &cur_obj) // called by load_models()
{
    glGenVertexArrays(1, &cur_obj.vao);
    glGenBuffers(1, &cur_obj.vbo);
    
    glBindVertexArray(cur_obj.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, cur_obj.vbo);
    glBufferData(GL_ARRAY_BUFFER, cur_obj.vao_vertices.size() * sizeof(Object::Vertex), &cur_obj.vao_vertices[0], GL_STATIC_DRAW);
    
    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)0);
    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, Normal));
    // Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, TexCoords));
    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, Tangent));
    // BiTangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Object::Vertex), (GLvoid*)offsetof(Object::Vertex, BiTangent));
    
    glBindVertexArray(0);
}
