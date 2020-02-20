//
//  Lighting.h
//  OpenGLProjects
//
//  Created by Dayuan Chen on 2/18/20.
//  Copyright © 2020 N33MO. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Lighting
{
    struct Direction_Light
    {
        bool status;
        glm::vec3 direction;
        
        nanogui::Color ambient;
        nanogui::Color diffuse;
        nanogui::Color specular;
    };
    
    struct Point_Light
    {
        bool status;
        glm::vec3 position;
        
        nanogui::Color ambient;
        nanogui::Color diffuse;
        nanogui::Color specular;
        
        float constant;
        float linear;
        float quadratic;
    };
    
public:
    Direction_Light direction_light;
    Point_Light point_light;
    
    Lighting() {}
    ~Lighting() {}
    
    void init()
    {
        direction_light.status = false;
        direction_light.direction = glm::vec3( -1.0f, -1.0f, -1.0f );
        direction_light.ambient = Eigen::Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );
        direction_light.diffuse = Eigen::Vector4f( 0.6f, 0.6f, 0.6f, 1.0f );
        direction_light.specular = Eigen::Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );
        
        point_light.status = false;
        point_light.position = glm::vec3( 1.2f, 1.0f, 2.0f );
        point_light.ambient = Eigen::Vector4f( 0.0f, 0.0f, 0.0f, 1.0f );
        point_light.diffuse = Eigen::Vector4f( 1.0f, 1.0f, 1.0f, 1.0f );
        point_light.specular = Eigen::Vector4f( 1.0f, 1.0f, 1.0f, 1.0f );
        point_light.constant = 1.0f;
        point_light.linear = 0.09f;
        point_light.quadratic = 0.032f;
    }
    
    void point_light_rotate(int axis, glm::vec3 objPos)
    {
        int a = (axis+1)%3;
        int b = (axis+2)%3;
        float tmp1 = point_light.position[a];
        float tmp2 = point_light.position[b];
        point_light.position[a] = (tmp1 - objPos[a]) * cos(glm::radians(1.0f)) - (tmp2 - objPos[b]) * sin(glm::radians(1.0f)) + objPos[a];
        point_light.position[b] = (tmp2 - objPos[b]) * cos(glm::radians(1.0f)) + (tmp1 - objPos[a]) * sin(glm::radians(1.0f)) + objPos[b];
    }
};
