#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

class Bone_Animation
{
public:
	Bone_Animation();
	~Bone_Animation();

	void init();
	void update(float delta_time);
	void reset();

public:

	// Here the head of each vector is the root bone
	std::vector<glm::vec3> scale_vector;
	std::vector<glm::vec3> rotation_degree_vector;
	std::vector<glm::vec4> colors;

	glm::vec3 root_position;
    
    // //
    std::vector<glm::vec3> rotate_bones;
    glm::vec3 target_position;
    glm::vec3 roottop_position;
    glm::vec3 upp_joint_length;
    glm::vec3 mid_joint_length;
    glm::vec3 bot_joint_length;
    glm::vec4 end_effector;
    glm::vec4 upp_joint_pos;
    glm::vec4 mid_joint_pos;
    glm::vec4 bot_joint_pos;
    
    glm::mat4 upp_joint_axis_mat4;
    glm::mat4 mid_joint_axis_mat4;
    glm::mat4 bot_joint_axis_mat4;
    glm::mat3 upp_joint_axis_mat3;
    glm::mat3 mid_joint_axis_mat3;
    glm::mat3 bot_joint_axis_mat3;
    
    std::vector<glm::vec3> world_axis;
    
    bool bone_move = false;
    glm::mat4 target_cube_obj_mat;
    glm::mat4 upp_bone_obj_mat;
    glm::mat4 mid_bone_obj_mat;
    glm::mat4 bot_bone_obj_mat;
    glm::mat4 upp_bone_obj_mat_scaled;
    glm::mat4 mid_bone_obj_mat_scaled;
    glm::mat4 bot_bone_obj_mat_scaled;
};

