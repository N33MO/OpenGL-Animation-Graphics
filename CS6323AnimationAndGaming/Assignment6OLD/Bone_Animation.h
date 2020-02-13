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
    bool bone_move = false;
    glm::vec3 target_position;
    glm::vec3 roottop_position;
    glm::vec3 roottop_upp_trans;
    glm::vec3 roottop_upptop_trans;
    glm::vec3 upptop_mid_trans;
    glm::vec3 upptop_midtop_trans;
    glm::vec3 midtop_bot_trans;
    glm::vec3 midtop_bottop_trans;
    std::vector<glm::vec3> rotate_bones;
    std::vector<glm::vec3> rotate_axis = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    
    glm::mat4 target_cube_obj_mat;
    glm::mat4 upp_bone_obj_mat;
    glm::mat4 mid_bone_obj_mat;
    glm::mat4 bot_bone_obj_mat;
    glm::vec4 target_point;
    glm::vec4 upp_end_point;
    glm::vec4 upp_transformed;
    glm::vec4 mid_end_point;
    glm::vec4 mid_transformed;
    glm::vec4 bot_end_point;
    glm::vec4 bot_transformed;

};

