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
    target_position = { 3.0f, 8.0f, 3.0f };
    roottop_position = { 2.0f, 1.5f, 2.0f };
    roottop_upp_trans = { 0.0f, 2.0f, 0.0f };
    roottop_upptop_trans = { 0.0f, 4.0f, 0.0f };
    upptop_mid_trans = { 0.0f, 1.5f, 0.0f };
    upptop_midtop_trans = { 0.0f, 3.0f, 0.0f };
    midtop_bot_trans = { 0.0f, 1.0f, 0.0f };
    midtop_bottop_trans = { 0.0f, 2.0f, 0.0f };
    target_point = glm::vec4(target_position, 1.0f);
    
    upp_end_point = glm::vec4(glm::vec3(0.0f),1.0f);
    upp_transformed = glm::vec4(glm::vec3(0.0f),1.0f);
    mid_end_point = glm::vec4(glm::vec3(0.0f),1.0f);
    mid_transformed = glm::vec4(glm::vec3(0.0f),1.0f);
    bot_end_point = glm::vec4(glm::vec3(0.0f),1.0f);
    bot_transformed = glm::vec4(glm::vec3(0.0f),1.0f);
    
    upp_bone_obj_mat = glm::mat4(1.0f);
    mid_bone_obj_mat = glm::mat4(1.0f);
    bot_bone_obj_mat = glm::mat4(1.0f);

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
		{0.7f,0.0f,0.0f,1.0f},  // red
		{0.7f,0.7f,0.0f,1.0f},  // yellow
		{0.7f,0.0f,0.7f,1.0f},  // purple
		{0.0f,0.7f,0.7f,1.0f},  // blue
        {0.2f,1.0f,0.2f,1.0f}   // green
	};
    
    // //
    rotate_bones = rotation_degree_vector;

}

void Bone_Animation::update(float delta_time)
{
    // //
    
    target_cube_obj_mat = glm::mat4(1.0f);
    target_cube_obj_mat = glm::translate(target_cube_obj_mat, target_position);
    
    upp_bone_obj_mat = glm::mat4(1.0f);
    mid_bone_obj_mat = glm::mat4(1.0f);
    bot_bone_obj_mat = glm::mat4(1.0f);
    upp_bone_obj_mat = glm::translate(upp_bone_obj_mat, roottop_position);
    mid_bone_obj_mat = glm::translate(mid_bone_obj_mat, roottop_position);
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, roottop_position);
    // .1.2 ROTATE all three bones accord to upp_bone
    for (int i = 2; i >= 0; i--)    // for loop rotate three bones, specific sequence of 'i' denotes y -> z -> x
    {
        upp_bone_obj_mat = glm::rotate(upp_bone_obj_mat, glm::radians(rotate_bones[1][(i+1)%3]), rotate_axis[(i+1)%3]);
        mid_bone_obj_mat = glm::rotate(mid_bone_obj_mat, glm::radians(rotate_bones[1][(i+1)%3]), rotate_axis[(i+1)%3]);
        bot_bone_obj_mat = glm::rotate(bot_bone_obj_mat, glm::radians(rotate_bones[1][(i+1)%3]), rotate_axis[(i+1)%3]);
    }
    // .1.3 TRANS first bone to correct pos
    upp_bone_obj_mat = glm::translate(upp_bone_obj_mat, roottop_upp_trans);
    // .2.1 TRANS last two bones to the second joint
    mid_bone_obj_mat = glm::translate(mid_bone_obj_mat, roottop_upptop_trans);
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, roottop_upptop_trans);
    // .2.2 ROTATE last two bones accord to mid_bone
    for (int i = 2; i >= 0; i--)    // for loop rotate two bones, specific sequence of 'i' denotes y -> z -> x
    {
        mid_bone_obj_mat = glm::rotate(mid_bone_obj_mat, glm::radians(rotate_bones[2][(i+1)%3]), rotate_axis[(i+1)%3]);
        bot_bone_obj_mat = glm::rotate(bot_bone_obj_mat, glm::radians(rotate_bones[2][(i+1)%3]), rotate_axis[(i+1)%3]);
    }
    // .2.3 TRANS second bone to correct pos
    mid_bone_obj_mat = glm::translate(mid_bone_obj_mat, upptop_mid_trans);
    // .3.1 TRANS final bone to the third joint
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, upptop_midtop_trans);
    // .3.2 ROTATE final bone accord to bot_bone
    for (int i = 2; i >= 0; i--)    // for loop rotate last bone, specific sequence of 'i' denotes y -> z -> x
    {
        bot_bone_obj_mat = glm::rotate(bot_bone_obj_mat, glm::radians(rotate_bones[3][(i+1)%3]), rotate_axis[(i+1)%3]);
    }
    // .3.3 TRANS final bone to correct pos
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, midtop_bot_trans);
    // .3.4 caculate end_point pos for IK
    upp_transformed = glm::translate(upp_bone_obj_mat, roottop_upp_trans) * upp_end_point;
    mid_transformed = glm::translate(mid_bone_obj_mat, upptop_mid_trans) * mid_end_point;
    bot_transformed = glm::translate(bot_bone_obj_mat, midtop_bot_trans) * bot_end_point;
    // .4 SCALE three bones
    upp_bone_obj_mat = glm::scale(upp_bone_obj_mat, scale_vector[1]);
    mid_bone_obj_mat = glm::scale(mid_bone_obj_mat, scale_vector[2]);
    bot_bone_obj_mat = glm::scale(bot_bone_obj_mat, scale_vector[3]);
    
    
    // Inverse Kinematics
    float dist = pow((bot_transformed[0]-target_position[0]),2) + pow((bot_transformed[1]-target_position[1]),2) + pow((bot_transformed[2]-target_position[2]),2);
//    std::cout << dist << std::endl;
    float threshold = pow(10, -6);
    if (bone_move == true && dist > threshold) {
//        std::cout << "to far." << std::endl;
        // .1 Calculate the Jacobian Matric 'jac'
        glm::vec3 upp_joint_pos = roottop_position;
        glm::vec3 mid_joint_pos = {upp_transformed.x, upp_transformed.y, upp_transformed.z};
        glm::vec3 bot_joint_pos = {mid_transformed.x, mid_transformed.y, mid_transformed.z};
        glm::vec3 end_effector = {bot_transformed.x, bot_transformed.y, bot_transformed.z};
        glm::vec3 goal_pos = target_position;
        // upp
        glm::vec3 axis_upp = glm::cross(end_effector - upp_joint_pos, goal_pos - upp_joint_pos);   // axis of upper joint
        axis_upp = glm::normalize(axis_upp);
        glm::vec3 d_upp = glm::cross(axis_upp, end_effector - upp_joint_pos);
        // mid
        glm::vec3 axis_mid = glm::cross(end_effector - mid_joint_pos, goal_pos - mid_joint_pos);
        axis_mid = glm::normalize(axis_mid);
        
        glm::vec3 d_mid = glm::cross(axis_mid, end_effector - mid_joint_pos);
        // bot
        glm::vec3 axis_bot = glm::cross(end_effector - bot_joint_pos, goal_pos - bot_joint_pos);
        axis_bot = glm::normalize(axis_bot);
        
        glm::vec3 d_bot = glm::cross(axis_bot, end_effector - bot_joint_pos);
        // 3 1-DOF
        glm::vec4 axis_upp_x = glm::transpose(glm::translate(upp_bone_obj_mat, -roottop_upp_trans))[0];
        glm::vec4 axis_upp_y = glm::transpose(glm::translate(upp_bone_obj_mat, -roottop_upp_trans))[1];
        glm::vec4 axis_upp_z = glm::transpose(glm::translate(upp_bone_obj_mat, -roottop_upp_trans))[2];
        glm::vec4 axis_mid_x = glm::transpose(glm::translate(mid_bone_obj_mat, -upptop_mid_trans))[0];
        glm::vec4 axis_mid_y = glm::transpose(glm::translate(mid_bone_obj_mat, -upptop_mid_trans))[1];
        glm::vec4 axis_mid_z = glm::transpose(glm::translate(mid_bone_obj_mat, -upptop_mid_trans))[2];
        glm::vec4 axis_bot_x = glm::transpose(glm::translate(bot_bone_obj_mat, -midtop_bot_trans))[0];
        glm::vec4 axis_bot_y = glm::transpose(glm::translate(bot_bone_obj_mat, -midtop_bot_trans))[1];
        glm::vec4 axis_bot_z = glm::transpose(glm::translate(bot_bone_obj_mat, -midtop_bot_trans))[2];
        
        glm::vec3 d_upp_x = glm::cross({axis_upp_x[0], axis_upp_x[1], axis_upp_x[2]}, (end_effector - upp_joint_pos));
        glm::vec3 d_mid_x = glm::cross({axis_mid_x[0], axis_mid_x[1], axis_mid_x[2]}, (end_effector - mid_joint_pos));
        glm::vec3 d_bot_x = glm::cross({axis_bot_x[0], axis_bot_x[1], axis_bot_x[2]}, (end_effector - bot_joint_pos));
        
        glm::vec3 d_upp_y = glm::cross({axis_upp_y[0], axis_upp_y[1], axis_upp_y[2]}, (end_effector - upp_joint_pos));
        glm::vec3 d_mid_y = glm::cross({axis_mid_y[0], axis_mid_y[1], axis_mid_y[2]}, (end_effector - mid_joint_pos));
        glm::vec3 d_bot_y = glm::cross({axis_bot_y[0], axis_bot_y[1], axis_bot_y[2]}, (end_effector - bot_joint_pos));
        
        glm::vec3 d_upp_z = glm::cross({axis_upp_z[0], axis_upp_z[1], axis_upp_z[2]}, (end_effector - upp_joint_pos));
        glm::vec3 d_mid_z = glm::cross({axis_mid_z[0], axis_mid_z[1], axis_mid_z[2]}, (end_effector - mid_joint_pos));
        glm::vec3 d_bot_z = glm::cross({axis_bot_z[0], axis_bot_z[1], axis_bot_z[2]}, (end_effector - bot_joint_pos));
        
        // Jacobian
        glm::mat3 jac = {d_upp, d_mid, d_bot};
        jac = glm::transpose(jac);
        glm::mat3 jac_t = glm::transpose(jac);
        
        glm::mat3 jac_axis_upp = {d_upp_x, d_upp_y, d_upp_z};
        glm::mat3 jac_axis_mid = {d_mid_x, d_mid_y, d_mid_z};
        glm::mat3 jac_axis_bot = {d_bot_x, d_bot_y, d_bot_z};
        jac_axis_upp = glm::transpose(jac_axis_upp);
        jac_axis_mid = glm::transpose(jac_axis_mid);
        jac_axis_bot = glm::transpose(jac_axis_bot);
        
        glm::mat3 jac_axis_upp_t = glm::transpose(jac_axis_upp);
        glm::mat3 jac_axis_mid_t = glm::transpose(jac_axis_mid);
        glm::mat3 jac_axis_bot_t = glm::transpose(jac_axis_bot);
        
        // .2 Calculate the step size
        float step_size;
        step_size = glm::pow(glm::length(jac_t * (target_position - end_effector)), 2) / glm::pow(glm::length(jac * jac_t * (target_position - end_effector)), 2);
        
        float step_size_upp;
        float step_size_mid;
        float step_size_bot;
        step_size_upp = glm::pow(glm::length(jac_axis_upp_t * (target_position - end_effector)), 2) / glm::pow(glm::length(jac_axis_upp * jac_axis_upp_t * (target_position - end_effector)), 2);
        step_size_mid = glm::pow(glm::length(jac_axis_mid_t * (target_position - end_effector)), 2) / glm::pow(glm::length(jac_axis_mid * jac_axis_mid_t * (target_position - end_effector)), 2);
        step_size_bot = glm::pow(glm::length(jac_axis_bot_t * (target_position - end_effector)), 2) / glm::pow(glm::length(jac_axis_bot * jac_axis_bot_t * (target_position - end_effector)), 2);
        
        glm::vec3 delta_e = goal_pos - end_effector;
        
        glm::vec3 change_res_upp = step_size_upp * jac_axis_upp_t * delta_e;
        glm::vec3 change_res_mid = step_size_mid * jac_axis_mid_t * delta_e;
        glm::vec3 change_res_bot = step_size_bot * jac_axis_bot_t * delta_e;
        std::cout << "Node change_res:" << std::endl;
        std::cout << "upp.x: " << change_res_upp[0] << " mid.x: " << change_res_mid[0] << " bot.x: " << change_res_bot[0] << std::endl;
        std::cout << "upp.y: " << change_res_upp[1] << " mid.y: " << change_res_mid[1] << " bot.y: " << change_res_bot[1] << std::endl;
        std::cout << "upp.z: " << change_res_upp[2] << " mid.z: " << change_res_mid[2] << " bot.z: " << change_res_bot[2] << std::endl;
        std::cout << std::endl << std::endl;
        
        // .3 Update 9 DOF bone values using the Transpose of 'jac' and step size;
        glm::mat3 change_res = step_size * jac_t;
//        std::cout << "Node change_res:" << std::endl;
//        std::cout << "[0][0]: " << change_res[0][0] << " [0][1]: " << change_res[0][1] << " [0][2]: " << change_res[0][2] << std::endl;
//        std::cout << "[1][0]: " << change_res[1][0] << " [1][1]: " << change_res[1][1] << " [1][2]: " << change_res[1][2] << std::endl;
//        std::cout << "[2][0]: " << change_res[2][0] << " [2][1]: " << change_res[2][1] << " [2][2]: " << change_res[2][2] << std::endl;
//        std::cout << std::endl << std::endl;
        glm::mat3 change_res_t = glm::transpose(change_res);
        
//        glm::vec3 degree_change_axis_upp;
//        glm::vec3 degree_change_axis_mid;
//        glm::vec3 degree_change_axis_bot;
        glm::mat3 degree_change_axis;
        
        float frac_top, frac_bot;
        glm::vec2 va,vb;
        // upp
        for (int j = 0; j < 3; j++) {   // j : 0 , 1 , 2 => joint: upp , mid , bot
            for (int i = 0; i < 3 ; i++) {  // i: 0 , 1 , 2 => axis: z , x , y
                va = {mid_joint_pos[i]-upp_joint_pos[i], mid_joint_pos[(i+1)%3]-upp_joint_pos[(i+1)%3]};
                vb = {mid_joint_pos[i]-upp_joint_pos[i]+change_res_t[0][i], mid_joint_pos[(i+1)%3]-upp_joint_pos[(i+1)%3]+change_res_t[0][(i+1)%3]};
                frac_top = va[0] * vb[0] + va[1] * vb[1];
                frac_bot = glm::length(va) * glm::length(vb);
                degree_change_axis[j][(i+2)%3] = glm::acos(frac_top/frac_bot);
            }
        }
//        std::cout << "Node change_deg:" << std::endl;
//        std::cout << "[0][0]: " << degree_change_axis[0][0] << " [0][1]: " << degree_change_axis[0][1] << " [0][2]: " << degree_change_axis[0][2] << std::endl;
//        std::cout << "[1][0]: " << degree_change_axis[1][0] << " [1][1]: " << degree_change_axis[1][1] << " [1][2]: " << degree_change_axis[1][2] << std::endl;
//        std::cout << "[2][0]: " << degree_change_axis[2][0] << " [2][1]: " << degree_change_axis[2][1] << " [2][2]: " << degree_change_axis[2][2] << std::endl;
//        std::cout << std::endl << std::endl;
        
//        degree_change_axis = glm::transpose(degree_change_axis);
        
        
        // .4 Update the end effector position , to 'rotat_bones'
        std::vector<glm::vec3> rotate_bones_IK = {{0.0f,0.0f,0.0f}, change_res_upp, change_res_mid, change_res_bot};
//        rotate_bones[1] += rotate_bones_IK[1];
//        rotate_bones[2] += rotate_bones_IK[2];
//        rotate_bones[3] += rotate_bones_IK[3];
//        rotate_bones[1] += degree_change_axis[0];
//        rotate_bones[2] += degree_change_axis[1];
//        rotate_bones[3] += degree_change_axis[2];
        
        
    }
    
    
    
}

void Bone_Animation::reset()
{
    // //
    rotate_bones = rotation_degree_vector;
}

