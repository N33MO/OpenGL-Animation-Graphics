#include "Curve.h"

struct point
{
    float x,y,z;
};

Curve::Curve()
{
}

Curve::~Curve()
{
}

void Curve::init()
{
	this->control_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};
//	calculate_curve();
}

void Curve::calculate_curve()
{
	this->curve_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};
    
    // Apply Catmull-Rom Formulation
    axis_x = control_points_pos[0][0];
    axis_y = control_points_pos[0][1];
    axis_z = control_points_pos[0][2];
    float abs = 0;
    int num_of_init_points = int( control_points_pos.size() );
    for (int i = 0; i < num_of_init_points; i++) {
        point p0,p1,v0,v1;
        p0.x = control_points_pos[i].x;
        p1.x = control_points_pos[(i+1) % num_of_init_points].x;
        v0.x = tau * ( control_points_pos[(i+1) % num_of_init_points].x - control_points_pos[(i-1+num_of_init_points) % num_of_init_points].x );
        v1.x = tau * ( control_points_pos[(i+2) % num_of_init_points].x - control_points_pos[(i-0) % num_of_init_points].x );
        
        p0.y = control_points_pos[i].y;
        p1.y = control_points_pos[(i+1) % num_of_init_points].y;
        v0.y = tau * ( control_points_pos[(i+1) % num_of_init_points].y - control_points_pos[(i-1+num_of_init_points) % num_of_init_points].y );
        v1.y = tau * ( control_points_pos[(i+2) % num_of_init_points].y - control_points_pos[(i-0) % num_of_init_points].y );
        
        p0.z = control_points_pos[i].z;
        p1.z = control_points_pos[(i+1) % num_of_init_points].z;
        v0.z = tau * ( control_points_pos[(i+1) % num_of_init_points].z - control_points_pos[(i-1+num_of_init_points) % num_of_init_points].z );
        v1.z = tau * ( control_points_pos[(i+2) % num_of_init_points].z - control_points_pos[(i-0) % num_of_init_points].z );
        
        // calculate points between each two given points.
        float delta = 1.0 / num_points_per_segment;
        for (int j = 0; j < num_points_per_segment; j++) {
            float t = j * delta;
            
            float t1 = 2 * pow(t, 3) + (-3) * pow(t, 2) + 1;
            float t2 = (-2) * pow(t, 3) + 3 * pow(t, 2);
            float t3 = 1 * pow(t, 3) + (-2) * pow(t, 2) + t;
            float t4 = 1 * pow(t, 3) + (-1) * pow(t, 2);
            
            abs += sqrt( pow(p0.x * t1 + p1.x * t2 + v0.x * t3 + v1.x * t4 - axis_x, 2)
                        + pow(p0.y * t1 + p1.y * t2 + v0.y * t3 + v1.y * t4 - axis_y, 2)
                        + pow(p0.z * t1 + p1.z * t2 + v0.z * t3 + v1.z * t4 - axis_z, 2)
                        );
            
//            std::cout << "testpoint curve len: (" << j << ") "<< abs << std::endl;
            
            len.push_back( abs );
            
            axis_x = p0.x * t1 + p1.x * t2 + v0.x * t3 + v1.x * t4;
            axis_y = p0.y * t1 + p1.y * t2 + v0.y * t3 + v1.y * t4;
            axis_z = p0.z * t1 + p1.z * t2 + v0.z * t3 + v1.z * t4;
            
            p.push_back( { axis_x, axis_y, axis_z } );
        }
    }
    
    // len vector normalization
    int num = len.size();
    float norm = len[num-1];
    for(int i = 0; i < num; i++) {
        len[i] /= norm;
//        std::cout << "testpoint curve len-dif: (" << i << ") "<< len[i]<< std::endl;
    }
    
}
