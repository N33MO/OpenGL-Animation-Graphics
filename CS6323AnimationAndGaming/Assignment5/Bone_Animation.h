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
    glm::vec3 roottop_position;
    glm::vec3 root_upp_trans;
    glm::vec3 root_upptop_trans;
    glm::vec3 upptop_mid_trans;
    glm::vec3 upptop_midtop_trans;
    glm::vec3 midtop_bot_trans;
    std::vector<glm::vec3> rotate_bones;
    std::vector<glm::vec3> rotate_axis = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

};

