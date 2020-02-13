#pragma once
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    
    std::vector<glm::vec3> control_points_pos;
    std::vector<glm::vec3> curve_points_pos;
    std::vector<glm::vec3> p;
};
