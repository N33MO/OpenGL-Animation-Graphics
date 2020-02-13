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
        {0.2f,1.0f,0.2f,1.0f}   // green
	};
    
    rotate_bones = rotation_degree_vector;
    target_position = { 3.0f , 8.0f , 3.0f };
    roottop_position = { 2.0f, 1.5f, 2.0f };    // root cube top center position, first rotational center
    upp_joint_length = { 0.0f, scale_vector[1][1] , 0.0f};  // upp joint length
    mid_joint_length = { 0.0f, scale_vector[2][1] , 0.0f};  // mid joint length
    bot_joint_length = { 0.0f, scale_vector[3][1] , 0.0f};  // bot joint length
//    end_effector = glm::vec4(glm::vec3(0.0f),1.0f);   // initialization of end effector position
    world_axis = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };    // for three basic axis coordinates' reference


}

void Bone_Animation::update(float delta_time)
{
    // //
    // set target cube position
    target_cube_obj_mat = glm::mat4(1.0f);
    target_cube_obj_mat = glm::translate(target_cube_obj_mat, target_position);
    
    // (8 steps + 1 EXTRA) update three bones/joints using Forward Kinematics (FK)
    upp_bone_obj_mat = glm::mat4(1.0f);
    mid_bone_obj_mat = glm::mat4(1.0f);
    bot_bone_obj_mat = glm::mat4(1.0f);
    // .1 translate all bones/joints to first rotational center, which is root cube's top center position
    upp_bone_obj_mat = glm::translate(upp_bone_obj_mat, roottop_position);
    mid_bone_obj_mat = glm::translate(mid_bone_obj_mat, roottop_position);
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, roottop_position);
    // .2 rotate all bones/joints according to first bone/joint's rotation degree
    for (int i = 2; i >= 0; i--) {  // rotate three bones/joints, for y -> z -> x order, rotate sequence would be: x -> z -> y (y <- z <- x)
        upp_bone_obj_mat = glm::rotate(upp_bone_obj_mat, glm::radians(rotate_bones[1][(i+1)%3]), world_axis[(i+1)%3]);
        mid_bone_obj_mat = glm::rotate(mid_bone_obj_mat, glm::radians(rotate_bones[1][(i+1)%3]), world_axis[(i+1)%3]);
        bot_bone_obj_mat = glm::rotate(bot_bone_obj_mat, glm::radians(rotate_bones[1][(i+1)%3]), world_axis[(i+1)%3]);
    }
    // .3 translate first bone/joint to correct position, that put up 0.5 * (upp joint length) AND translate other two bones/joints to second rotational center
    upp_bone_obj_mat = glm::translate(upp_bone_obj_mat, 0.5f * upp_joint_length);
    mid_bone_obj_mat = glm::translate(mid_bone_obj_mat, upp_joint_length);
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, upp_joint_length);
    // .4 rotate two remain bones/joints according to second bone/joint's rotation degree
    for (int i = 2; i >=0; i--) {   // rotate detail same as above
        mid_bone_obj_mat = glm::rotate(mid_bone_obj_mat, glm::radians(rotate_bones[2][(i+1)%3]), world_axis[(i+1)%3]);
        bot_bone_obj_mat = glm::rotate(bot_bone_obj_mat, glm::radians(rotate_bones[2][(i+1)%3]), world_axis[(i+1)%3]);
    }
    // .5 translate second bone/joints to correct position, that put up 0.5 * (mid joint length) AND translate last bone/joint to final rotational center
    mid_bone_obj_mat = glm::translate(mid_bone_obj_mat, 0.5f * mid_joint_length);
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, mid_joint_length);
    // .6 rotate the final bone/joint according to itselves bone/joint's rotation degree
    for (int i = 2; i >=0; i--) {   // rotate detail same as above
        bot_bone_obj_mat = glm::rotate(bot_bone_obj_mat, glm::radians(rotate_bones[3][(i+1)%3]), world_axis[(i+1)%3]);
    }
    // .7 translate final bone/joint to correct position, that put up 0.5 * (bot joint length)
    bot_bone_obj_mat = glm::translate(bot_bone_obj_mat, 0.5f * bot_joint_length);
    // .8 scale three bones/joints
    upp_bone_obj_mat_scaled = glm::scale(upp_bone_obj_mat, scale_vector[1]);
    mid_bone_obj_mat_scaled = glm::scale(mid_bone_obj_mat, scale_vector[2]);
    bot_bone_obj_mat_scaled = glm::scale(bot_bone_obj_mat, scale_vector[3]);
    
    // .EXTRA calculate end effector position & joints position for IK use, this should be done before scaling
    glm::mat4 end_effector_pos_mat4 = glm::translate(bot_bone_obj_mat, 0.5f * bot_joint_length);
    glm::mat4 upp_joint_pos_mat4 = glm::translate(upp_bone_obj_mat, -0.5f * upp_joint_length);
    glm::mat4 mid_joint_pos_mat4 = glm::translate(mid_bone_obj_mat, -0.5f * mid_joint_length);
    glm::mat4 bot_joint_pos_mat4 = glm::translate(bot_bone_obj_mat, -0.5f * bot_joint_length);
    end_effector = end_effector_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    upp_joint_pos = upp_joint_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    mid_joint_pos = mid_joint_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    bot_joint_pos = bot_joint_pos_mat4 * glm::vec4(glm::vec3(0.0f),1.0f);
    
//    std::cout << "joint positions: " << std::endl;
//    std::cout << "end effector: " << std::endl;
//    std::cout << end_effector.x << "\t" << end_effector.y << "\t" << end_effector.z << std::endl;
//    std::cout << "upp: " << std::endl;
//    std::cout << upp_joint_pos.x << "\t" << upp_joint_pos.y << "\t" << upp_joint_pos.z << std::endl;
//    std::cout << "mid: " << std::endl;
//    std::cout << mid_joint_pos.x << "\t" << mid_joint_pos.y << "\t" << mid_joint_pos.z << std::endl;
//    std::cout << "bot: " << std::endl;
//    std::cout << bot_joint_pos.x << "\t" << bot_joint_pos.y << "\t" << bot_joint_pos.z << std::endl;
//    std::cout << std::endl;
    
    upp_joint_axis_mat4 = upp_joint_pos_mat4 * glm::mat4(1.0f);
    mid_joint_axis_mat4 = mid_joint_pos_mat4 * glm::mat4(1.0f);
    bot_joint_axis_mat4 = bot_joint_pos_mat4 * glm::mat4(1.0f);
    // for the mat3(s) below, xyz axis are represented by 123 rows AFTER we transpose them
    upp_joint_axis_mat3 = { upp_joint_axis_mat4[0][0], upp_joint_axis_mat4[0][1], upp_joint_axis_mat4[0][2],
                            upp_joint_axis_mat4[1][0], upp_joint_axis_mat4[1][1], upp_joint_axis_mat4[1][2],
                            upp_joint_axis_mat4[2][0], upp_joint_axis_mat4[2][1], upp_joint_axis_mat4[2][2]};
    mid_joint_axis_mat3 = { mid_joint_axis_mat4[0][0], mid_joint_axis_mat4[0][1], mid_joint_axis_mat4[0][2],
                            mid_joint_axis_mat4[1][0], mid_joint_axis_mat4[1][1], mid_joint_axis_mat4[1][2],
                            mid_joint_axis_mat4[2][0], mid_joint_axis_mat4[2][1], mid_joint_axis_mat4[2][2]};
    bot_joint_axis_mat3 = { bot_joint_axis_mat4[0][0], bot_joint_axis_mat4[0][1], bot_joint_axis_mat4[0][2],
                            bot_joint_axis_mat4[1][0], bot_joint_axis_mat4[1][1], bot_joint_axis_mat4[1][2],
                            bot_joint_axis_mat4[2][0], bot_joint_axis_mat4[2][1], bot_joint_axis_mat4[2][2]};
    upp_joint_axis_mat3 = glm::transpose(upp_joint_axis_mat3);
    mid_joint_axis_mat3 = glm::transpose(mid_joint_axis_mat3);
    bot_joint_axis_mat3 = glm::transpose(bot_joint_axis_mat3);
    
//    std::cout << "joint axis mat3: " << std::endl;
//    std::cout << "upp: " << std::endl;
//    std::cout << upp_joint_axis_mat3[0][0] << "\t"  << upp_joint_axis_mat3[0][1] << "\t"  << upp_joint_axis_mat3[0][2] << std::endl;
//    std::cout << upp_joint_axis_mat3[1][0] << "\t"  << upp_joint_axis_mat3[1][1] << "\t"  << upp_joint_axis_mat3[1][2] << std::endl;
//    std::cout << upp_joint_axis_mat3[2][0] << "\t"  << upp_joint_axis_mat3[2][1] << "\t"  << upp_joint_axis_mat3[2][2] << std::endl;
//    std::cout << std::endl;
//    std::cout << "mid: " << std::endl;
//    std::cout << mid_joint_axis_mat3[0][0] << "\t"  << mid_joint_axis_mat3[0][1] << "\t"  << mid_joint_axis_mat3[0][2] << std::endl;
//    std::cout << mid_joint_axis_mat3[1][0] << "\t"  << mid_joint_axis_mat3[1][1] << "\t"  << mid_joint_axis_mat3[1][2] << std::endl;
//    std::cout << mid_joint_axis_mat3[2][0] << "\t"  << mid_joint_axis_mat3[2][1] << "\t"  << mid_joint_axis_mat3[2][2] << std::endl;
//    std::cout << std::endl;
//    std::cout << "bot: " << std::endl;
//    std::cout << bot_joint_axis_mat3[0][0] << "\t"  << bot_joint_axis_mat3[0][1] << "\t"  << bot_joint_axis_mat3[0][2] << std::endl;
//    std::cout << bot_joint_axis_mat3[1][0] << "\t"  << bot_joint_axis_mat3[1][1] << "\t"  << bot_joint_axis_mat3[1][2] << std::endl;
//    std::cout << bot_joint_axis_mat3[2][0] << "\t"  << bot_joint_axis_mat3[2][1] << "\t"  << bot_joint_axis_mat3[2][2] << std::endl;
//    std::cout << std::endl;
    
    // ---------- FK above ----------
    // ---------- IK below ----------
    
    /*  quick notes
     
     - 1. Calculate Jacobian Matrix
        Jacobian Matrix = [∂e/∂ø]
     
        ∂e / ∂Φ = a' x (e - r');        // for ROTATIONAL DOFs.  a': unit length rotation axis , e: end effector position, r': joint pivot postion
     
?       calculation of a', two methods
            method 1: 3 DOF Rotational Joints ( easy to apply with FK )
                        a' = glm::translate( (corresponding)transform_mat , -0.5f * (corresponding)joint_length ) * (corresponding)world_axis
            method 2: Quaternion Joints
                        a' = normalize((e - r') x (g - r'))
     
     - 2. Calculate step size
                           ||Jac_t * (g - e)||^2
        step size: ⍺ = -----------------------------;       // Jac_t: transpose of Jacobian Matrix, g: target position
                        ||Jac * Jac_t * (g - e)||^2
     
?    - 3. Update 9 DOF bone values
        ∆Φ = ⍺ * Jac_t * ∆e     // apply with method 1 in a' calculation
     
?    - 4. Update end effector position 'e'
        (rotational degree matrix in FK) += ∆Φ
     
     */
    
    // .1 caculate Jacobian Matrix
    
        // vec3 is needed to do cross products
    glm::vec3 end_effector_vec3 = {end_effector.x, end_effector.y, end_effector.z};
    glm::vec3 upp_joint_pos_vec3 = {upp_joint_pos.x, upp_joint_pos.y, upp_joint_pos.z};
    glm::vec3 mid_joint_pos_vec3 = {mid_joint_pos.x, mid_joint_pos.y, mid_joint_pos.z};
    glm::vec3 bot_joint_pos_vec3 = {bot_joint_pos.x, bot_joint_pos.y, bot_joint_pos.z};
        // get some basic variables
    
        // BEGIN method 1: 3 DOF Rotational Joints - Three Jacobian Matrix ( each matrix for one axis of three joints)
    glm::mat3 Jac_axis_x_t = {  glm::cross(upp_joint_axis_mat3[0] , end_effector_vec3 - upp_joint_pos_vec3),
                                glm::cross(mid_joint_axis_mat3[0] , end_effector_vec3 - mid_joint_pos_vec3),
                                glm::cross(bot_joint_axis_mat3[0] , end_effector_vec3 - bot_joint_pos_vec3)  };
    glm::mat3 Jac_axis_y_t = {  glm::cross(upp_joint_axis_mat3[1] , end_effector_vec3 - upp_joint_pos_vec3),
                                glm::cross(mid_joint_axis_mat3[1] , end_effector_vec3 - mid_joint_pos_vec3),
                                glm::cross(bot_joint_axis_mat3[1] , end_effector_vec3 - bot_joint_pos_vec3)  };
    glm::mat3 Jac_axis_z_t = {  glm::cross(upp_joint_axis_mat3[2] , end_effector_vec3 - upp_joint_pos_vec3),
                                glm::cross(mid_joint_axis_mat3[2] , end_effector_vec3 - mid_joint_pos_vec3),
                                glm::cross(bot_joint_axis_mat3[2] , end_effector_vec3 - bot_joint_pos_vec3)  };
    glm::mat3 Jac_axis_x = glm::transpose(Jac_axis_x_t);
    glm::mat3 Jac_axis_y = glm::transpose(Jac_axis_y_t);
    glm::mat3 Jac_axis_z = glm::transpose(Jac_axis_z_t);
    
    // .1 MODIFIED 3 x 9 Jacobian Matrix
    glm::vec3 Jac_upp_x = glm::cross(upp_joint_axis_mat3[0], end_effector_vec3 - upp_joint_pos_vec3);
    glm::vec3 Jac_upp_y = glm::cross(upp_joint_axis_mat3[1], end_effector_vec3 - upp_joint_pos_vec3);
    glm::vec3 Jac_upp_z = glm::cross(upp_joint_axis_mat3[2], end_effector_vec3 - upp_joint_pos_vec3);
    glm::vec3 Jac_mid_x = glm::cross(mid_joint_axis_mat3[0], end_effector_vec3 - mid_joint_pos_vec3);
    glm::vec3 Jac_mid_y = glm::cross(mid_joint_axis_mat3[1], end_effector_vec3 - mid_joint_pos_vec3);
    glm::vec3 Jac_mid_z = glm::cross(mid_joint_axis_mat3[2], end_effector_vec3 - mid_joint_pos_vec3);
    glm::vec3 Jac_bot_x = glm::cross(bot_joint_axis_mat3[0], end_effector_vec3 - bot_joint_pos_vec3);
    glm::vec3 Jac_bot_y = glm::cross(bot_joint_axis_mat3[1], end_effector_vec3 - bot_joint_pos_vec3);
    glm::vec3 Jac_bot_z = glm::cross(bot_joint_axis_mat3[2], end_effector_vec3 - bot_joint_pos_vec3);
    
    
//    std::cout << "Jacobian Matrix: " << std::endl;
//    std::cout << "axis_x: " << std::endl;
//    std::cout << Jac_axis_x[0][0] << Jac_axis_x[0][1] << Jac_axis_x[0][2] << std::endl;
//    std::cout << Jac_axis_x[1][0] << Jac_axis_x[1][1] << Jac_axis_x[1][2] << std::endl;
//    std::cout << Jac_axis_x[2][0] << Jac_axis_x[2][1] << Jac_axis_x[2][2] << std::endl;
//    std::cout << std::endl;
//    std::cout << "axis_y: " << std::endl;
//    std::cout << Jac_axis_y[0][0] << Jac_axis_y[0][1] << Jac_axis_y[0][2] << std::endl;
//    std::cout << Jac_axis_y[1][0] << Jac_axis_y[1][1] << Jac_axis_y[1][2] << std::endl;
//    std::cout << Jac_axis_y[2][0] << Jac_axis_y[2][1] << Jac_axis_y[2][2] << std::endl;
//    std::cout << std::endl;
//    std::cout << "axis_z: " << std::endl;
//    std::cout << Jac_axis_z[0][0] << Jac_axis_z[0][1] << Jac_axis_z[0][2] << std::endl;
//    std::cout << Jac_axis_z[1][0] << Jac_axis_z[1][1] << Jac_axis_z[1][2] << std::endl;
//    std::cout << Jac_axis_z[2][0] << Jac_axis_z[2][1] << Jac_axis_z[2][2] << std::endl;
//    std::cout << std::endl;
        // END   method 1: get 3 Jacobian Matrix, each one for an axis(x, y, and z) for three joints
    
//        // BEGIN method 2: quaternion calculation
//    glm::vec3 upp_joint_axis_vec3 = glm::normalize(glm::cross(end_effector_vec3 - upp_joint_pos_vec3, target_position - upp_joint_pos_vec3));
//    glm::vec3 mid_joint_axis_vec3 = glm::normalize(glm::cross(end_effector_vec3 - mid_joint_pos_vec3, target_position - mid_joint_pos_vec3));
//    glm::vec3 bot_joint_axis_vec3 = glm::normalize(glm::cross(end_effector_vec3 - bot_joint_pos_vec3, target_position - bot_joint_pos_vec3));
//
//    glm::vec4 upp_joint_axis_quat = { upp_joint_axis_vec3.x , upp_joint_axis_vec3.y , upp_joint_axis_vec3.z , 0.0f };
//    glm::vec4 mid_joint_axis_quat = { mid_joint_axis_vec3.x , mid_joint_axis_vec3.y , mid_joint_axis_vec3.z , 0.0f };
//    glm::vec4 bot_joint_axis_quat = { bot_joint_axis_vec3.x , bot_joint_axis_vec3.y , bot_joint_axis_vec3.z , 0.0f };
//        // END   method 2: get quaternion rotational axis for each joint
    
    // .2 calculate step size
    glm::vec3 delta_e = target_position - end_effector_vec3;    // delta_e: g - e
//    glm::float32 step_size_axis_x = glm::pow(glm::length(Jac_axis_x_t * delta_e), 2) / glm::pow(glm::length(Jac_axis_x * Jac_axis_x_t * delta_e), 2);
//    glm::float32 step_size_axis_y = glm::pow(glm::length(Jac_axis_y_t * delta_e), 2) / glm::pow(glm::length(Jac_axis_y * Jac_axis_y_t * delta_e), 2);
//    glm::float32 step_size_axis_z = glm::pow(glm::length(Jac_axis_y_t * delta_e), 2) / glm::pow(glm::length(Jac_axis_z * Jac_axis_z_t * delta_e), 2);
    
    // .2 MODIFIED
    std::vector<glm::vec3> Jac = {  Jac_upp_x, Jac_upp_y, Jac_upp_z,
                                    Jac_mid_x, Jac_mid_y, Jac_mid_z,
                                    Jac_bot_x, Jac_bot_y, Jac_bot_z    };
    
    std::vector<float> numerator = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < Jac.size(); i++) {
        numerator[i] = glm::dot(Jac[i], delta_e);
    }
    std::vector<float> denominator = {0, 0, 0};
    for (int i = 0; i < Jac.size(); i++) {
        denominator[0] += Jac[i][0] * numerator[i];
        denominator[1] += Jac[i][1] * numerator[i];
        denominator[2] += Jac[i][2] * numerator[i];
    }

    glm::float32 numerator_normsqr = 0;
    glm::float32 denominator_normsqr = 0;
    for (int i = 0; i < numerator.size(); i++) {
        numerator_normsqr += glm::pow(numerator[i],2);
    }
    for (int i = 0; i < denominator.size(); i++) {
        denominator_normsqr += glm::pow(denominator[i],2);
    }
    glm::float32 step_size = numerator_normsqr / denominator_normsqr;
    
    
//    std::cout << "step size for each Jacobian Matrix: " << std::endl;
//    std::cout << step_size_axis_x << "\t" << step_size_axis_y << "\t" << step_size_axis_z << std::endl;
//    std::cout << std::endl;
    
//    // .3 update rotate degrees & end effector position
//    glm::vec3 change_res_axis_x = step_size_axis_x * Jac_axis_x * delta_e;
//    glm::vec3 change_res_axis_y = step_size_axis_y * Jac_axis_y * delta_e;
//    glm::vec3 change_res_axis_z = step_size_axis_z * Jac_axis_z * delta_e;
    
    // .3 MODIFIED
    std::vector<glm::float32> change_res_deg = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < Jac.size(); i++) {
        change_res_deg[i] = glm::dot(Jac[i], delta_e) * step_size;
    }
    
//    std::cout <<    change_res_deg[0] << "\t" << change_res_deg[1] << "\t" << change_res_deg[2] << "\n" <<
//                    change_res_deg[3] << "\t" << change_res_deg[4] << "\t" << change_res_deg[5] << "\n" <<
//    change_res_deg[6] << "\t" << change_res_deg[7] << "\t" << change_res_deg[8] << "\n" << std::endl;
    
//    std::cout << "final degree changes of 9 DOF: " << std::endl;
//    std::cout << change_res_axis_x[0] << "\t" << change_res_axis_x[1] << "\t" << change_res_axis_x[2] << std::endl;
//    std::cout << change_res_axis_y[0] << "\t" << change_res_axis_y[1] << "\t" << change_res_axis_y[2] << std::endl;
//    std::cout << change_res_axis_z[0] << "\t" << change_res_axis_z[1] << "\t" << change_res_axis_z[2] << std::endl;
//    std::cout << std::endl;
    
    glm::float32 dist = glm::distance(target_position, end_effector_vec3);
    glm::float32 threshold = 1e-6;
//    if (bone_move == true && dist > threshold) {
//        rotate_bones[1][0] += change_res_axis_x[0]; rotate_bones[1][1] += change_res_axis_y[0]; rotate_bones[1][2] += change_res_axis_z[0];
//        rotate_bones[2][0] += change_res_axis_x[1]; rotate_bones[2][1] += change_res_axis_y[1]; rotate_bones[2][2] += change_res_axis_z[1];
//        rotate_bones[3][0] += change_res_axis_x[2]; rotate_bones[3][1] += change_res_axis_y[2]; rotate_bones[3][2] += change_res_axis_z[2];
//    }
    
    // MODIFIED degree change
    if (bone_move == true && dist > threshold) {
        rotate_bones[1][0] += change_res_deg[0]; rotate_bones[1][1] += change_res_deg[1]; rotate_bones[1][2] += change_res_deg[2];
        rotate_bones[2][0] += change_res_deg[3]; rotate_bones[2][1] += change_res_deg[4]; rotate_bones[2][2] += change_res_deg[5];
        rotate_bones[3][0] += change_res_deg[6]; rotate_bones[3][1] += change_res_deg[7]; rotate_bones[3][2] += change_res_deg[8];
    }
    
}

void Bone_Animation::reset()
{
    // //
    rotate_bones = rotation_degree_vector;
}

