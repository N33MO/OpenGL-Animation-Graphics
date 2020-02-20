//
//  Camera.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 1/31/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#pragma once

#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROTATE_X_UP,
    ROTATE_X_DOWN,
    ROTATE_Y_UP,
    ROTATE_Y_DOWN,
    ROTATE_Z_UP,
    ROTATE_Z_DOWN
};

class Camera
{
public:
    // Camera view parameters
    glm::vec3 ori_position;
    glm::vec3 ori_front;
    glm::vec3 ori_up;
    glm::vec3 ori_right;
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    
    // Camera projection parameters
    float ori_zoom;
    float zoom;
    
    float near;
    float far;
    
    unsigned int width;
    unsigned int height;
    
    // // object center vector
    glm::vec3 objCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    
    // Camera projection matrix: used for projection
    glm::mat4 proj_mat;
    
    // Camera View matrix: used for changing camera rotation and position
    // //
    glm::mat4 view_mat;
    
    // Camera parameter initialization
    Camera(
           glm::vec3 _position = glm::vec3( 0, 0.6, 3 ),
           glm::vec3 _front = glm::vec3( 0, 0, -1 ),
           glm::vec3 _up = glm::vec3( 0, 1, 0 ),
           glm::vec3 _right = glm::vec3( 1, 0, 0 ),
           float _zoom = 45.0f,
           float _near = 0.1f,
           float _far = 5.0f,
           unsigned int _width = 1600,
           unsigned int _height = 900
           )
    {
        this->ori_position = _position;
        this->ori_front = _front;
        this->ori_up = _up;
        this->ori_right = _right;
        this->ori_zoom = _zoom;
        this->near = _near;
        this->far = _far;
        this->width = _width;
        this->height = _height;
        this->view_mat = glm::lookAt(this->ori_position, this->ori_position, this->ori_up); // //
    }
    
    void init()
    {
        reset();
    }
    
    void reset()
    {
        this->position = ori_position;
        this->front = ori_front;
        this->up = ori_up;
        this->right = ori_right;
        this->zoom = ori_zoom;
    }
    
    void gui_btn_reset()
    {
        this->front = ori_front;
        this->up = ori_up;
        this->right = ori_right;
        this->zoom = ori_zoom;
    }
    
    void process_keyboard(Camera_Movement direction, GLfloat delta_time)
    {
        GLfloat move_velocity = delta_time * 10.0f;
        GLfloat rotate_velocity = delta_time * 50.0f;
        
        if( direction == FORWARD )
            this->position += this->front * move_velocity;
        if( direction == BACKWARD )
            this->position -= this->front * move_velocity;
        if( direction == LEFT )
            this->position -= this->right * move_velocity;
        if( direction == RIGHT )
            this->position += this->right * move_velocity;
        if( direction == UP )
            this->position += this->up * move_velocity;
        if( direction == DOWN )
            this->position -= this->up * move_velocity;
        if( direction == ROTATE_X_UP )
            rotate_x(rotate_velocity);
        if( direction == ROTATE_X_DOWN )
            rotate_x(-rotate_velocity);
        if( direction == ROTATE_Y_UP )
            rotate_y(rotate_velocity);
        if( direction == ROTATE_Y_DOWN )
            rotate_y(-rotate_velocity);
        if( direction == ROTATE_Z_UP )
            rotate_z(rotate_velocity);
        if( direction == ROTATE_Z_DOWN )
            rotate_z(-rotate_velocity);
    }
    
    // Rotate specific angle along local camera system(LCS)
    void rotate_x(GLfloat angle)
    {
        glm::vec3 up = this->up;
        glm::mat4 rotation_mat(1.0f);
        rotation_mat = glm::rotate(rotation_mat, glm::radians(angle), this->right);
        this->up = glm::normalize(glm::vec3(rotation_mat * glm::vec4(up, 1.0f)));
        this->front = glm::normalize(glm::cross(this->up, this->right));
    }
    
    void rotate_y(GLfloat angle)
    {
        glm::vec3 front = this->front;
        glm::mat4 rotation_mat(1.0f);
        rotation_mat = glm::rotate(rotation_mat, glm::radians(angle), this->up);
        this->front = glm::normalize(glm::vec3(rotation_mat * glm::vec4(front, 1.0f)));
        this->right = glm::normalize(glm::cross(this->front, this->up));
    }
    
    void rotate_z(GLfloat angle)
    {
        glm::vec3 right = this->right;
        glm::mat4 rotation_mat(1.0f);
        rotation_mat = glm::rotate(rotation_mat, glm::radians(angle), this->front);
        this->right = glm::normalize(glm::vec3(rotation_mat * glm::vec4(right, 1.0f)));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
    
    // Get camera view matrix
    glm::mat4 get_view_mat()
    {
        this->view_mat = glm::lookAt(this->position, this->position + this->front, this->up); // // &center: objCenter
        return this->view_mat;
    }
    
    // Get camera projection matrix
    glm::mat4 get_projection_mat()
    {
        this->proj_mat = glm::perspective(this->zoom, (GLfloat)this->width / (GLfloat)this->height, this->near, this->far);
        return this->proj_mat;
    }
};
