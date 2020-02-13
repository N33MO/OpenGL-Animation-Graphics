#include "Aircraft_Animation.h"


Aircraft_Animation::Aircraft_Animation()
{
	this->m_model_mat = glm::mat4(1.0f);
}


Aircraft_Animation::~Aircraft_Animation()
{
}

void Aircraft_Animation::init()
{
	reset();
}

void Aircraft_Animation::init(Curve* animation_curve)
{
	m_animation_curve = animation_curve;
	reset();
}

float s;
int i = 0;
int seg;
void Aircraft_Animation::update(float delta_time)
{
    v0 = 2 / (t2 - t1 + 1);
    if (cb_move) {
        if (t < 1.0) {
            t = t + delta_time / total_moving_time;
        }
        else
            t = 1.0;
    }
    
    m_model_mat = glm::mat4(1.0f);
    
    if ( t < t1 ) {         //  t < t1, aircraft is accelerating, t is always greater than 0
        s = v0 * pow(t, 2) / (2 * t1);
        while (m_animation_curve->len[i] < s) {
            i++;
        }
//        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[i]);
        m_model_mat = m_model_mat * glm::mat4_cast(m_animation_curve->qt[i]);
    }
    else if ( t < t2 ) {    // t1 <= t < t2, aircraft keeps a constant velocity
        s = v0 * t1 / 2 + v0 * (t - t1);
        while (m_animation_curve->len[i] < s) {
            i++;
        }
//        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[i]);
        m_model_mat = m_model_mat * glm::mat4_cast(m_animation_curve->qt[i]);
    }
    else if ( t < 1 ) {     // t2 <= t < 1, aircraft is decelerating
        s = 1 + (v0 * pow(t-1, 2)) / (2*(t2-1));
        while (m_animation_curve->len[i] < s && i < m_animation_curve->p.size()-1) {
            i++;
        }
//        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[i]);
        m_model_mat = m_model_mat * glm::mat4_cast(m_animation_curve->qt[i]);
    }
    else {  // 1 <= t, aircraft stops moving at the original point. For this program we make the aircraft moves only one round
        s = 1.0;
        i = 0;
//        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[0]);
        m_model_mat = m_model_mat * glm::mat4_cast(m_animation_curve->qt[i]);
    }
    
    
//    // Reference from FLAG 1B to FLAB 1E
//    // FLAG 1B // Using Slerp to do the Interpolation
//    seg = i / m_animation_curve->num_points_per_segment;
//    m_animation_curve->qt.push_back( glm::slerp(m_animation_curve->control_points_quaternion[seg],
//                                                m_animation_curve->control_points_quaternion[(seg+1) % m_animation_curve->control_points_quaternion.size()],
//                                                t) );
//    // Rotate the target vector
//    // v' = qt * v;
//    m_model_mat = m_model_mat * glm::mat4_cast(m_animation_curve->qt[0]);
//    m_animation_curve->qt.pop_back();
    
//    float a,b,c,d;
//    a = m_animation_curve->qt[0].w;
//    b = m_animation_curve->qt[0].x;
//    c = m_animation_curve->qt[0].y;
//    d = m_animation_curve->qt[0].z;
//    std::vector<glm::vec3> q( { { 1-2*c*c-2*d*d, 2*b*c-2*a*d, 2*a*c+2*b*d } ,
//                                { 2*b*c+2*a*d, 1-2*b*b-2*d*d, 2*c*d-2*a*b } ,
//                                { 2*b*d-2*a*c, 2*a*b+2*c*d, 1-2*b*b-2*c*c } } );
//
//    float temp_x, temp_y, temp_z;
//    temp_x = (1-2*c*c-2*d*d)*m_animation_curve->p[i].x + (2*b*c-2*a*d)*m_animation_curve->p[i].y + (2*a*c+2*b*d)*m_animation_curve->p[i].z;
//    temp_y = (2*b*c+2*a*d)*m_animation_curve->p[i].x + (1-2*b*b-2*d*d)*m_animation_curve->p[i].y + (2*c*d-2*a*b)*m_animation_curve->p[i].z;
//    temp_z = (2*b*d-2*a*c)*m_animation_curve->p[i].x + (2*a*b+2*c*d)*m_animation_curve->p[i].y + (1-2*b*b-2*c*c)*m_animation_curve->p[i].z;
//    std::vector<glm::vec3> vp;
//    vp.push_back( { temp_x, temp_y, temp_z } );
//
//    vp.pop_back();
    
//    // test point
//    std::cout << "t = " << t << " " <<
//                 "qt: w = " << a <<
//                 " , x = " << b <<
//                 " , y = " << c <<
//                 " , z = " << d <<
//                std::endl;
//    // FLAG 1E
    
}

void Aircraft_Animation::reset()
{
	m_model_mat = glm::mat4(1.0f);
	if (m_animation_curve != nullptr && m_animation_curve->control_points_pos.size() > 0)
	{
        s = 0.0;
        t = 0.0;
        i = 0;
		m_model_mat = glm::translate(m_model_mat, m_animation_curve->control_points_pos[0]);
	}
}
