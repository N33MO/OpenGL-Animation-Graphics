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
    // //
    roottop_position = { 2.0f, 1.5f, 2.0f };
    root_upp_trans = { 0.0f, 2.0f, 0.0f };
    root_upptop_trans = { 0.0f, 4.0f, 0.0f };
    upptop_mid_trans = { 0.0f, 1.5f, 0.0f };
    upptop_midtop_trans = { 0.0f, 3.0f, 0.0f };
    midtop_bot_trans = { 0.0f, 1.0f, 0.0f };

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
    
    // //
    rotate_bones = rotation_degree_vector;

}

void Bone_Animation::update(float delta_time)
{
    // //
//    rotate_bones;
}

void Bone_Animation::reset()
{
    // //
    rotate_bones = rotation_degree_vector;
}

