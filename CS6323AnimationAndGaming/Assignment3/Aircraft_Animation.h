#pragma once

#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Curve.h"

class Aircraft_Animation
{

public:
	float total_moving_time = 10;
    float v0;
	float t1 = 0.3;
	float t2 = 0.9;
//    v0 = 2 / (t2 - t1 + 1);

private:
	glm::mat4 m_model_mat;
	Curve* m_animation_curve = nullptr;

public:
	Aircraft_Animation();
	~Aircraft_Animation();

	void init();
	void init(Curve* animation_curve);
    
    bool cb_move = false;

	void update(float delta_time);

	void reset();
	glm::mat4 get_model_mat() { return m_model_mat; };
};

