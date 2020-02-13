#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265358979323846

class Bone_Animation
{
public:
	Bone_Animation();
	~Bone_Animation();

	void init();
	void update(float delta_time);
	void reset();
    
    float deg_to_rad = 2 * PI / 360;

public:

	// Here the head of each vector is the root bone
	std::vector<glm::vec3> scale_vector;
	std::vector<glm::vec3> rotation_degree_vector;
	std::vector<glm::vec4> colors;

	glm::vec3 root_position;
    
    std::vector<glm::vec3> rotate_bones;
    glm::vec3 upper_position;
    glm::vec3 middle_position;
    glm::vec3 bottom_position;
    
    glm::vec3 root_upper_joint;
    glm::vec3 upper_root_joint;
    glm::vec3 upper_middle_joint;
    glm::vec3 middle_upper_joint;
    glm::vec3 middle_bottom_joint;
    glm::vec3 bottom_middle_joint;
    
    glm::vec3 upper_trans_position;
    glm::vec3 middle_trans_position;
    glm::vec3 bottom_trans_position;
    
    std::vector<glm::vec3> axis_origin = { { 1.0f, 0.0f, 0.0f },
                                           { 0.0f, 1.0f, 0.0f },
                                           { 0.0f, 0.0f, 1.0f } };
    
    std::vector<glm::vec3> axis_upper = { { 1.0f, 0.0f, 0.0f },
                                          { 0.0f, 1.0f, 0.0f },
                                          { 0.0f, 0.0f, 1.0f } };
    
    std::vector<glm::vec3> axis_middle = { { 1.0f, 0.0f, 0.0f },
                                           { 0.0f, 1.0f, 0.0f },
                                           { 0.0f, 0.0f, 1.0f } };
    
    std::vector<glm::vec3> axis_bottom = { { 1.0f, 0.0f, 0.0f },
                                           { 0.0f, 1.0f, 0.0f },
                                           { 0.0f, 0.0f, 1.0f } };
    

};

