#include "Bone_Animation.h"

Bone_Animation::Bone_Animation()
{
}


Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
	root_position = { 2.0f,1.0f,2.0f };
    upper_position = { 2.0f,3.5f,2.0f };
    middle_position = { 2.0f,7.0f,2.0f };
    bottom_position = { 2.0f,9.5f,2.0f };

	scale_vector =
	{
		{1.0f,1.0f,1.0f},
		{0.5f,4.0f,0.5f},
		{0.5f,3.0f,0.5f},
		{0.5f,2.0f,0.5f}
	};

	rotation_degree_vector = 
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	colors = 
	{
		{0.7f,0.0f,0.0f,1.0f},
		{0.7f,0.7f,0.0f,1.0f},
		{0.7f,0.0f,0.7f,1.0f},
		{0.0f,0.7f,0.7f,1.0f}
	};
    
    rotate_bones = rotation_degree_vector;
    
    root_upper_joint = { 2.0f, 1.5f, 2.0f };
    upper_root_joint = { 2.0f, 1.5f, 2.0f };
    upper_middle_joint = { 2.0f, 5.5f, 2.0f };
    middle_upper_joint = { 2.0f, 5.5f, 2.0f };
    middle_bottom_joint = { 2.0f, 8.5f, 2.0f };
    bottom_middle_joint = { 2.0f, 8.5f, 2.0f };
    
    upper_trans_position = upper_position;
    middle_trans_position = middle_position;
    bottom_trans_position = bottom_position;
    
}

void Bone_Animation::update(float delta_time)
{
    // Changing joint position
    
//    // calculate points rotation ordered by x -> z -> y ? but correspond to rotation order y -> z -> x ???
//    // upper - root      upper part
//    upper_root_joint.x = upper_position.x + 2.0 * cos(rotate_bones[1][0] * deg_to_rad) * sin(rotate_bones[1][2] * deg_to_rad);
//    upper_root_joint.y = upper_position.y - 2.0 * cos(rotate_bones[1][0] * deg_to_rad) * cos(rotate_bones[1][2] * deg_to_rad);
//    upper_root_joint.z = upper_position.z - 2.0 * sin(rotate_bones[1][0] * deg_to_rad);
//
////    upper_root_joint.x = upper_position.x + 2.0 * (cos(rotate_bones[1][1] * deg_to_rad) * cos(rotate_bones[1][0] * deg_to_rad) * sin(rotate_bones[1][2] * deg_to_rad)) + (sin(rotate_bones[1][1] * deg_to_rad) * sin(rotate_bones[1][0] * deg_to_rad));
////    upper_root_joint.y = upper_position.y - 2.0 * cos(rotate_bones[1][0] * deg_to_rad) * cos(rotate_bones[1][2] * deg_to_rad);
////    upper_root_joint.z = upper_position.z - 2.0 * (cos(rotate_bones[1][1] * deg_to_rad) * sin(rotate_bones[1][0] * deg_to_rad)) - (sin(rotate_bones[1][1] * deg_to_rad) * rotate_bones[1][1] * deg_to_rad);
//
//    upper_trans_position = upper_position + (root_upper_joint - upper_root_joint);
//
//    // upper - middle    upper part
//    upper_middle_joint.x = upper_trans_position.x - 2.0 * cos(rotate_bones[1][0] * deg_to_rad) * sin(rotate_bones[1][2] * deg_to_rad);
//    upper_middle_joint.y = upper_trans_position.y + 2.0 * cos(rotate_bones[1][0] * deg_to_rad) * cos(rotate_bones[1][2] * deg_to_rad);
//    upper_middle_joint.z = upper_trans_position.z + 2.0 * sin(rotate_bones[1][0] * deg_to_rad);
//
//    // middle - upper    middle part
//    middle_upper_joint.x = middle_position.x + 1.5 * cos((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad) * sin((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
//    middle_upper_joint.y = middle_position.y - 1.5 * cos((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad) * cos((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
//    middle_upper_joint.z = middle_position.z - 1.5 * sin((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
//    middle_trans_position = middle_position + (upper_middle_joint - middle_upper_joint);
//
//    // middle - bottom   middle part
//    middle_bottom_joint.x = middle_trans_position.x - 1.5 * cos((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad) * sin((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
//    middle_bottom_joint.y = middle_trans_position.y + 1.5 * cos((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad) * cos((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
//    middle_bottom_joint.z = middle_trans_position.z + 1.5 * sin((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
//
//    // bottom - middle   bottom part
//    bottom_middle_joint.x = bottom_position.x + 1.0 * cos((rotate_bones[3][0] + rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad) * sin((rotate_bones[3][2] + rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
//    bottom_middle_joint.y = bottom_position.y - 1.0 * cos((rotate_bones[3][0] + rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad) * cos((rotate_bones[3][2] + rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
//    bottom_middle_joint.z = bottom_position.z - 1.0 * sin((rotate_bones[3][0] + rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
//    bottom_trans_position = bottom_position + (middle_bottom_joint - bottom_middle_joint);
    
    
    // calculate points rotation ordered by y -> z -> x (without considering theta_Y) ? but correspond to rotation order x -> z -> y ???
    // upper - root     upper part
    upper_root_joint.x = upper_position.x + 2.0 * sin(rotate_bones[1][2] * deg_to_rad);
    upper_root_joint.y = upper_position.y - 2.0 * cos(rotate_bones[1][2] * deg_to_rad) * cos(rotate_bones[1][0] * deg_to_rad);
    upper_root_joint.z = upper_position.z - 2.0 * cos(rotate_bones[1][2] * deg_to_rad) * sin(rotate_bones[1][0] * deg_to_rad);
    upper_trans_position = upper_position + (root_upper_joint - upper_root_joint);
    // upper - middle   upper part
    upper_middle_joint.x = upper_trans_position.x - 2.0 * sin(rotate_bones[1][2] * deg_to_rad);
    upper_middle_joint.y = upper_trans_position.y + 2.0 * cos(rotate_bones[1][2] * deg_to_rad) * cos(rotate_bones[1][0] * deg_to_rad);
    upper_middle_joint.z = upper_trans_position.z + 2.0 * cos(rotate_bones[1][2] * deg_to_rad) * sin(rotate_bones[1][0] * deg_to_rad);

    // middle - upper   middle part
    middle_upper_joint.x = middle_position.x + 1.5 * sin((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
    middle_upper_joint.y = middle_position.y - 1.5 * cos((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad) * cos((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
    middle_upper_joint.z = middle_position.z - 1.5 * cos((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad) * sin((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
    middle_trans_position = middle_position + (upper_middle_joint - middle_upper_joint);
    // middle - bottom  middle part
    middle_bottom_joint.x = middle_trans_position.x - 1.5 * sin((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
    middle_bottom_joint.y = middle_trans_position.y + 1.5 * cos((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad) * cos((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
    middle_bottom_joint.z = middle_trans_position.z + 1.5 * cos((rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad) * sin((rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);

    // bottom - middle  bottom part
    bottom_middle_joint.x = bottom_position.x + 1.0 * sin((rotate_bones[3][2] + rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad);
    bottom_middle_joint.y = bottom_position.y - 1.0 * cos((rotate_bones[3][2] + rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad) * cos((rotate_bones[3][0] + rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
    bottom_middle_joint.z = bottom_position.z - 1.0 * cos((rotate_bones[3][2] + rotate_bones[2][2] + rotate_bones[1][2]) * deg_to_rad) * sin((rotate_bones[3][0] + rotate_bones[2][0] + rotate_bones[1][0]) * deg_to_rad);
    bottom_trans_position = bottom_position + (middle_bottom_joint - bottom_middle_joint);
    
    
    // Changing rotation axis
    
//    // rotation matrix template
//    std::vector<glm::vec3> rotation_x_up = { { 1, 0, 0 },
//                                             { 0, cos(rotation_degree_vector[1][0]), -sin(rotation_degree_vector[1][0]) },
//                                             { 0, sin(rotation_degree_vector[1][0]),  cos(rotation_degree_vector[1][0]) } };
//
//    std::vector<glm::vec3> rotation_y_up = { {  cos(rotation_degree_vector[1][1]), 0, sin(rotation_degree_vector[1][1]) },
//                                             {  0, 1, 0 },
//                                             { -sin(rotation_degree_vector[1][1]), 0, cos(rotation_degree_vector[1][1]) } };
//
//    std::vector<glm::vec3> rotation_z_up = { { cos(rotation_degree_vector[1][0]), -sin(rotation_degree_vector[1][0]), 0 },
//                                             { sin(rotation_degree_vector[1][0]),  cos(rotation_degree_vector[1][0]), 0 },
//                                             { 0, 0, 1 } };
    
    // upper part   axis unchanged
    axis_upper = axis_origin;
    
    // middle part  axis change accord to upper part
    axis_middle = axis_upper;   // y * z * x * vec3
    
    // bottom part  axis change with two former parts
    axis_bottom = axis_middle;
    
}

void Bone_Animation::reset()
{
    rotate_bones = rotation_degree_vector;
}

