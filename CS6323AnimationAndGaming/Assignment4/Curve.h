#pragma once
#include <vector>
#include <iostream>

 #define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class Curve
{
public:

	Curve();
	~Curve();
	
	void init();
	void calculate_curve();
	
public:
	float tau = 0.5; // Coefficient for catmull-rom spline
	int num_points_per_segment = 200;
    
    bool curve_switch = true;

	std::vector<glm::vec3> control_points_pos;
	std::vector<glm::vec3> curve_points_pos;
	std::vector<glm::quat> control_points_quaternion;
    
    std::vector<glm::vec3> p;
    float axis_x, axis_y, axis_z;
    std::vector<float> len;
    
    std::vector<glm::quat> qt;
};
