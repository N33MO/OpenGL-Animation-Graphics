#include "Curve.h"

struct point
{
    double x,y,z;
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
}

void Curve::calculate_curve()
{
//    std::cout << "Run func: calculate_curve." << std::endl;
    
//    // point check
//    int i = 0;
//    std::cout << "point check (point " << i << "): (" <<
//                control_points_pos[i].x << ", " <<
//                control_points_pos[i].y << ", " <<
//                control_points_pos[i].z << ")" <<
//                std::endl;
    
    this->curve_points_pos = {
        { 0.0, 8.5, -2.0 },  // 0
        { -3.0, 11.0, 2.3 }, // 1
        { -6.0, 8.5, -2.5 }, // 2
        { -4.0, 5.5, 2.8 },  // 3
        { 1.0, 2.0, -4.0 },  // 4
        { 4.0, 2.0, 3.0 },   // 5
        { 7.0, 8.0, -2.0 },  // 6
        { 3.0, 10.0, 3.7 }   // 7
    };
    
    // Apply Catmull-Rom Formulation
    int num_of_init_points = int( control_points_pos.size() );
    for (int i = 0; i < num_of_init_points; i++) {
//        p.push_back( control_points_pos[i] );
        point p0,p1,v0,v1;
        p0.x = control_points_pos[i].x;
        p1.x = control_points_pos[(i+1) % num_of_init_points].x;
        v0.x = tau * ( control_points_pos[(i+1) % num_of_init_points].x - control_points_pos[(i-1+num_of_init_points) % num_of_init_points].x );
        v1.x = tau * ( control_points_pos[(i+2) % num_of_init_points].x - control_points_pos[(i-0) % num_of_init_points].x );
        
//        std::cout << i << "th point, ** v0.x: " << v0.x << std::endl;
        
        p0.y = control_points_pos[i].y;
        p1.y = control_points_pos[(i+1) % num_of_init_points].y;
        v0.y = tau * ( control_points_pos[(i+1) % num_of_init_points].y - control_points_pos[(i-1+num_of_init_points) % num_of_init_points].y );
        v1.y = tau * ( control_points_pos[(i+2) % num_of_init_points].y - control_points_pos[(i-0) % num_of_init_points].y );
        
        p0.z = control_points_pos[i].z;
        p1.z = control_points_pos[(i+1) % num_of_init_points].z;
        v0.z = tau * ( control_points_pos[(i+1) % num_of_init_points].z - control_points_pos[(i-1+num_of_init_points) % num_of_init_points].z );
        v1.z = tau * ( control_points_pos[(i+2) % num_of_init_points].z - control_points_pos[(i-0) % num_of_init_points].z );
        
//        // test tangents
//        if (i == 0) {
//            std::cout << "For i = " << i << ", the tangent of x-y plane is:" << std::endl;
//            std::cout << "v0.x: " << v0.x << std::endl;
//            std::cout << "v0.y: " << v0.y << std::endl;
//        }
        
        // calculate points between each two given points.
        double delta = 1.0 / num_points_per_segment;
        for (int j = 0; j < num_points_per_segment; j++) {
            double t = j * delta;
//            std::cout << "t: " << t << std::endl; // test variation of 't'
            
            double t1 = 2 * pow(t, 3) + (-3) * pow(t, 2) + 1;
            double t2 = (-2) * pow(t, 3) + 3 * pow(t, 2);
            double t3 = 1 * pow(t, 3) + (-2) * pow(t, 2) + t;
            double t4 = 1 * pow(t, 3) + (-1) * pow(t, 2);
            
            p.push_back( {
                p0.x * t1 + p1.x * t2 + v0.x * t3 + v1.x * t4,
                p0.y * t1 + p1.y * t2 + v0.y * t3 + v1.y * t4,
                p0.z * t1 + p1.z * t2 + v0.z * t3 + v1.z * t4
            } );
        }
    }
    
//    std::cout << "Hermite Curve. Number of points: " << p.size() << std::endl; // check total points' numbers
    
//    // check several points
//    for (int i = 0; i < 10; i+=1) {
//        std::cout << "Test points " << i << ": (" << p[i].x << ", " << p[i].y << ", " << p[i].z << ")" << std::endl;
//    }
    
}
