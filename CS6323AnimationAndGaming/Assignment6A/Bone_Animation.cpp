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

	scale_vector =
	{
		{1.0f,1.0f,1.0f},   // root
		{0.5f,4.0f,0.5f},   // upp
		{0.5f,3.0f,0.5f},   // mid
		{0.5f,2.0f,0.5f}    // bot
	};

	rotation_degree_vector = 
	{
		{0.0f,0.0f,0.0f},   // root
		{0.0f,0.0f,30.0f},   // upp
		{0.0f,0.0f,30.0f},   // mid
		{0.0f,0.0f,30.0f}    // bot
	};

	colors = 
	{
		{0.7f,0.0f,0.0f,1.0f},  // red
		{0.7f,0.7f,0.0f,1.0f},  // yellow
		{0.7f,0.0f,0.7f,1.0f},  // purple
		{0.0f,0.7f,0.7f,1.0f},  // blue
        {0.2f,1.0f,0.2f,1.0f}   // green - for goal object
	};
    
    // //
    rotate_bones = rotation_degree_vector;   // create another matrix for 9 DOF rotational degree
    goal_position = { 3.0f, 8.0f, 3.0f }; // initialize target/goal cube location
    first_pivot_position = { 2.0f , 1.5f , 2.0f };
    // using scale vector to denote length of each joint
    upp_joint_length = { 0.0f , scale_vector[1][1] , 0.0f };
    mid_joint_length = { 0.0f , scale_vector[2][1] , 0.0f };
    bot_joint_length = { 0.0f , scale_vector[3][1] , 0.0f };
    
    absolute_axis = { {1,0,0} , {0,1,0} , {0,0,1} };   // use for rotational axis, each time these axis become the local rotational axis

}

void Bone_Animation::update(float delta_time)
{
    // //
    /*
     we need to use both vec3, vec4 & mat3, mat4 to do calculation
    */
    
    // set goal cube position
    goal_cube_obj_mat4 = glm::mat4(1.0f);
    goal_cube_obj_mat4 = glm::translate(goal_cube_obj_mat4, goal_position);
    
    // (8 steps) update three bones/joints using Forward Kinematics (FK)
    
    // .0 initialize bones/joint mat4 for transformation
    upp_bone_obj_mat4 = glm::mat4(1.0f);
    mid_bone_obj_mat4 = glm::mat4(1.0f);
    bot_bone_obj_mat4 = glm::mat4(1.0f);
    
    // .1 translate all bones/joints to first rotational center, which is root cube's top center position
    upp_bone_obj_mat4 = glm::translate(upp_bone_obj_mat4, first_pivot_position);
    mid_bone_obj_mat4 = glm::translate(mid_bone_obj_mat4, first_pivot_position);
    bot_bone_obj_mat4 = glm::translate(bot_bone_obj_mat4, first_pivot_position);
    
    // .2 rotate all bones/joints according to first bone/joint's rotation degree
    for (int i = 2; i >= 0; i--) {  // rotate three bones/joints, for y -> z -> x order, rotate sequence would be: x -> z -> y (y <- z <- x)
        upp_bone_obj_mat4 = glm::rotate(upp_bone_obj_mat4, glm::radians(rotate_bones[1][(i+1)%3]), absolute_axis[(i+1)%3]);
        mid_bone_obj_mat4 = glm::rotate(mid_bone_obj_mat4, glm::radians(rotate_bones[1][(i+1)%3]), absolute_axis[(i+1)%3]);
        bot_bone_obj_mat4 = glm::rotate(bot_bone_obj_mat4, glm::radians(rotate_bones[1][(i+1)%3]), absolute_axis[(i+1)%3]);
    }
    
    // .3 translate first bone/joint to correct position, that put up 0.5 * (upp joint length) on Y axis, AND translate other two bones/joints to second pivot/rotational center
    upp_bone_obj_mat4 = glm::translate(upp_bone_obj_mat4, 0.5f * upp_joint_length);
    mid_bone_obj_mat4 = glm::translate(mid_bone_obj_mat4, upp_joint_length);
    bot_bone_obj_mat4 = glm::translate(bot_bone_obj_mat4, upp_joint_length);
    
    // .4 rotate two remain bones/joints according to second bone/joint's rotation degree
    for (int i = 2; i >=0; i--) {   // rotation order same as above
        mid_bone_obj_mat4 = glm::rotate(mid_bone_obj_mat4, glm::radians(rotate_bones[2][(i+1)%3]), absolute_axis[(i+1)%3]);
        bot_bone_obj_mat4 = glm::rotate(bot_bone_obj_mat4, glm::radians(rotate_bones[2][(i+1)%3]), absolute_axis[(i+1)%3]);
    }
    
    // .5 translate second bone/joint to correct position, that put up 0.5 * (mid joint length) on Y axis, AND translate last bone/joint to final pivot/rotational center
    mid_bone_obj_mat4 = glm::translate(mid_bone_obj_mat4, 0.5f * mid_joint_length);
    bot_bone_obj_mat4 = glm::translate(bot_bone_obj_mat4, mid_joint_length);
    
    // .6 rotate the final bone/joint according to itselves bone/joint's rotation degree
    for (int i = 2; i >=0; i--) {   // rotate detail same as above
        bot_bone_obj_mat4 = glm::rotate(bot_bone_obj_mat4, glm::radians(rotate_bones[3][(i+1)%3]), absolute_axis[(i+1)%3]);
    }
    
    // .7 translate final bone/joint to correct position, that put up 0.5 * (bot joint length) on Y axis
    bot_bone_obj_mat4 = glm::translate(bot_bone_obj_mat4, 0.5f * bot_joint_length);
    
    // .8 scale three bones/joints using a new mat4, since we should use unscaled mat4 to calculate values like end effector & joint positions & joint pivots for IK implementation
    upp_bone_obj_mat4_scaled = glm::scale(upp_bone_obj_mat4, scale_vector[1]);
    mid_bone_obj_mat4_scaled = glm::scale(mid_bone_obj_mat4, scale_vector[2]);
    bot_bone_obj_mat4_scaled = glm::scale(bot_bone_obj_mat4, scale_vector[3]);
    
    
    // ---------------- FK above ----------------
    // ---------------- IK below ----------------
    
    /*  quick notes
         
     - 1. Calculate Jacobian Matrix
        Jacobian Matrix = [∂e/∂ø]
         
        ∂e / ∂Φ = a' x (e - r');
        // for ROTATIONAL DOFs.  a': unit length rotation axis , e: end effector position, r': joint pivot postion
         
        calculation of a', two methods, we used method 1 for this application
            method 1: 3 DOF Rotational Joints ( easy to apply with FK )
                a' = glm::translate( (corresponding)transform_mat , -0.5f * (corresponding)joint_length ) * (corresponding)world_axis
            method 2: Quaternion Joints
                a' = normalize((e - r') x (g - r'))
         
     - 2. Calculate step size
                           ||Jac_t * (g - e)||^2
        step size: ⍺ = -----------------------------;
                        ||Jac * Jac_t * (g - e)||^2
            // Jac_t: transpose of Jacobian Matrix, g: target position
         
     - 3. Update 9 DOF bone values
        ∆Φ = ⍺ * Jac_t * ∆e     // apply with method 1 in a' calculation
         
     - 4. Update end effector position 'e'
        (rotational degree matrix in FK) += ∆Φ
         
     */
    
    // .0 calculate end effector position & joint positions(mat4) & joint pivot axis using mat4 from FK
        // position mat4
    end_effector_pos_mat4 = glm::translate(bot_bone_obj_mat4, 0.5f * bot_joint_length);
    upp_joint_pos_mat4 = glm::translate(upp_bone_obj_mat4, -0.5f * upp_joint_length);
    mid_joint_pos_mat4 = glm::translate(mid_bone_obj_mat4, -0.5f * mid_joint_length);
    bot_joint_pos_mat4 = glm::translate(bot_bone_obj_mat4, -0.5f * bot_joint_length);
        // position vec4
    end_effector_vec4 = end_effector_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    upp_joint_pos_vec4 = upp_joint_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    mid_joint_pos_vec4 = mid_joint_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    bot_joint_pos_vec4 = bot_joint_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
        // position vec3
    end_effector_vec3  = { end_effector_vec4.x,  end_effector_vec4.y,  end_effector_vec4.z};
    upp_joint_pos_vec3 = {upp_joint_pos_vec4.x, upp_joint_pos_vec4.y, upp_joint_pos_vec4.z};
    mid_joint_pos_vec3 = {mid_joint_pos_vec4.x, mid_joint_pos_vec4.y, mid_joint_pos_vec4.z};
    bot_joint_pos_vec3 = {bot_joint_pos_vec4.x, bot_joint_pos_vec4.y, bot_joint_pos_vec4.z};
        // pivot axis mat4
    upp_joint_axis_mat4 = upp_joint_pos_mat4 * glm::mat4(1.0f);
    mid_joint_axis_mat4 = mid_joint_pos_mat4 * glm::mat4(1.0f);
    bot_joint_axis_mat4 = bot_joint_pos_mat4 * glm::mat4(1.0f);
        // pivot axis mat4, manualy fetch from mat4, and after transpose, row 123 represent axis xyz
    upp_joint_axis_mat3 = { upp_joint_axis_mat4[0][0], upp_joint_axis_mat4[0][1], upp_joint_axis_mat4[0][2],
                            upp_joint_axis_mat4[1][0], upp_joint_axis_mat4[1][1], upp_joint_axis_mat4[1][2],
                            upp_joint_axis_mat4[2][0], upp_joint_axis_mat4[2][1], upp_joint_axis_mat4[2][2]};
    mid_joint_axis_mat3 = { mid_joint_axis_mat4[0][0], mid_joint_axis_mat4[0][1], mid_joint_axis_mat4[0][2],
                            mid_joint_axis_mat4[1][0], mid_joint_axis_mat4[1][1], mid_joint_axis_mat4[1][2],
                            mid_joint_axis_mat4[2][0], mid_joint_axis_mat4[2][1], mid_joint_axis_mat4[2][2]};
    bot_joint_axis_mat3 = { bot_joint_axis_mat4[0][0], bot_joint_axis_mat4[0][1], bot_joint_axis_mat4[0][2],
                            bot_joint_axis_mat4[1][0], bot_joint_axis_mat4[1][1], bot_joint_axis_mat4[1][2],
                            bot_joint_axis_mat4[2][0], bot_joint_axis_mat4[2][1], bot_joint_axis_mat4[2][2]};
        // however in real implementation after output check, we should not transpose to get the xyz axis (reason unknown)
//    upp_joint_axis_mat3 = glm::transpose(upp_joint_axis_mat3);
//    mid_joint_axis_mat3 = glm::transpose(mid_joint_axis_mat3);
//    bot_joint_axis_mat3 = glm::transpose(bot_joint_axis_mat3);
                    
    
    // .1 calculate Jacobian Matrix, come up a 3 x 9 matrix (9 vec3)
    Jac_upp_x = glm::cross(upp_joint_axis_mat3[0], end_effector_vec3 - upp_joint_pos_vec3);
    Jac_upp_y = glm::cross(upp_joint_axis_mat3[1], end_effector_vec3 - upp_joint_pos_vec3);
    Jac_upp_z = glm::cross(upp_joint_axis_mat3[2], end_effector_vec3 - upp_joint_pos_vec3);
    Jac_mid_x = glm::cross(mid_joint_axis_mat3[0], end_effector_vec3 - mid_joint_pos_vec3);
    Jac_mid_y = glm::cross(mid_joint_axis_mat3[1], end_effector_vec3 - mid_joint_pos_vec3);
    Jac_mid_z = glm::cross(mid_joint_axis_mat3[2], end_effector_vec3 - mid_joint_pos_vec3);
    Jac_bot_x = glm::cross(bot_joint_axis_mat3[0], end_effector_vec3 - bot_joint_pos_vec3);
    Jac_bot_y = glm::cross(bot_joint_axis_mat3[1], end_effector_vec3 - bot_joint_pos_vec3);
    Jac_bot_z = glm::cross(bot_joint_axis_mat3[2], end_effector_vec3 - bot_joint_pos_vec3);
    
    // .2 calculate step size , check in quick notes
    delta_e = goal_position - end_effector_vec3;    // delta_e: g - e
        // integrate Jac into a vector for easier coding
    Jac = { Jac_upp_x , Jac_upp_y , Jac_upp_z ,
            Jac_mid_x , Jac_mid_y , Jac_mid_z ,
            Jac_bot_x , Jac_bot_y , Jac_bot_z };
        // calculate numerator and denominator seperately, numerator gets a 9x1 vec and denominator gets a 3x1 vec (3x9 x (9x3 x 3x1))
    numerator = { 0,0,0, 0,0,0, 0,0,0 };
    for (int i = 0;  i < Jac.size(); i++) {
        numerator[i] = glm::dot(Jac[i], delta_e);
    }
    denominator = { 0,0,0 };
    for (int i = 0; i < Jac.size(); i++) {
        denominator[0] += Jac[i][0] * numerator[i];
        denominator[1] += Jac[i][1] * numerator[i];
        denominator[2] += Jac[i][2] * numerator[i];
    }
    numerator_normsqr = 0;
    denominator_normsqr = 0;
    for (int i = 0; i < numerator.size(); i++) {
        numerator_normsqr += glm::pow(numerator[i], 2);
    }
    for (int i = 0; i < denominator.size(); i++) {
        denominator_normsqr += glm::pow(denominator[i], 2);
    }
    step_size = numerator_normsqr / denominator_normsqr;
    
    // .3 update rotate degrees, end efector position will be updated in next loop in the starter place(step.0)
    change_res_deg = { 0,0,0, 0,0,0, 0,0,0 };
    for (int i = 0;  i < Jac.size(); i++) {
        change_res_deg[i] = step_size * glm::dot(Jac[i], delta_e);
    }
    dist = glm::distance(goal_position, end_effector_vec3);
//    threshold;
    if (bone_move == true && dist > threshold) {
        rotate_bones[1][0] += change_res_deg[0]; rotate_bones[1][1] += change_res_deg[1]; rotate_bones[1][2] += change_res_deg[2];
        rotate_bones[2][0] += change_res_deg[3]; rotate_bones[2][1] += change_res_deg[4]; rotate_bones[2][2] += change_res_deg[5];
        rotate_bones[3][0] += change_res_deg[6]; rotate_bones[3][1] += change_res_deg[7]; rotate_bones[3][2] += change_res_deg[8];
    }
}

void Bone_Animation::reset()
{
    // //
    rotate_bones = rotation_degree_vector;  // reset 9 DOF rotational degrees
}

