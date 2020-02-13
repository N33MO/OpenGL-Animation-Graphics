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
    // init()
    std::vector<glm::vec3> rotate_bones;
    glm::vec3 goal_position;
    glm::vec3 first_pivot_position;
    glm::vec3 upp_joint_length;
    glm::vec3 mid_joint_length;
    glm::vec3 bot_joint_length;
    std::vector<glm::vec3> absolute_axis;
    
    // update()
    glm::mat4 goal_cube_obj_mat4;
    // FK
    glm::mat4 upp_bone_obj_mat4;
    glm::mat4 mid_bone_obj_mat4;
    glm::mat4 bot_bone_obj_mat4;
    glm::mat4 upp_bone_obj_mat4_scaled;
    glm::mat4 mid_bone_obj_mat4_scaled;
    glm::mat4 bot_bone_obj_mat4_scaled;
    // IK
        // position mat4
    glm::mat4 end_effector_pos_mat4;
    glm::mat4 upp_joint_pos_mat4;
    glm::mat4 mid_joint_pos_mat4;
    glm::mat4 bot_joint_pos_mat4;
        // position vec4
    glm::vec4 end_effector_vec4;
    glm::vec4 upp_joint_pos_vec4;
    glm::vec4 mid_joint_pos_vec4;
    glm::vec4 bot_joint_pos_vec4;
        // position vec3
    glm::vec3 end_effector_vec3;
    glm::vec3 upp_joint_pos_vec3;
    glm::vec3 mid_joint_pos_vec3;
    glm::vec3 bot_joint_pos_vec3;
        // pivot axis mat4
    glm::mat4 upp_joint_axis_mat4;
    glm::mat4 mid_joint_axis_mat4;
    glm::mat4 bot_joint_axis_mat4;
        // pivot axis mat3
    glm::mat3 upp_joint_axis_mat3;
    glm::mat3 mid_joint_axis_mat3;
    glm::mat3 bot_joint_axis_mat3;
        // Jacobian Matrixs, for 3 joints with 3 DOF of each joint, total 9 columns in Jac. create 9 vec3 for 9 columns
    glm::vec3 Jac_upp_x;
    glm::vec3 Jac_upp_y;
    glm::vec3 Jac_upp_z;
    glm::vec3 Jac_mid_x;
    glm::vec3 Jac_mid_y;
    glm::vec3 Jac_mid_z;
    glm::vec3 Jac_bot_x;
    glm::vec3 Jac_bot_y;
    glm::vec3 Jac_bot_z;
        // Jacobian Matrix save in a vector (size = 9) for easier coding
    std::vector<glm::vec3> Jac;
        // step size numerator & denominator
    glm::vec3 delta_e;
    std::vector<float> numerator;
    std::vector<float> denominator;
    glm::float32 numerator_normsqr;
    glm::float32 denominator_normsqr;
    glm::float32 step_size;
        // changes of 9 DOF rotate degree
    std::vector<glm::float32> change_res_deg;
        // loop condition
    glm::float32 dist;
    glm::float32 threshold = 1e-6;
        // switch in nanogui bar
    bool bone_move = false;

};

