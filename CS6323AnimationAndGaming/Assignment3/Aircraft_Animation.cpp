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

float t = 0;    // t is the current time lapse since the aircraft starts moving
float s;        // s calculate the total distance from 0 to t. using ' d - t ' function
int i = 0;      // i as a pointer denotes the corresponding i th points which have a total distance of s
void Aircraft_Animation::update(float delta_time)
{
    v0 = 2 / (t2 - t1 + 1); // originally t1 = 0.3, t2 = 0.9 in header files, nanogui provide further changes of these two values
    if (cb_move) {  // if the checkbox is 'checked', calculate 't' for further movements
        t = t + delta_time / total_moving_time;
    }
    
    if ( t < t1 ) {         //  t < t1, aircraft is accelerating, t is always greater than 0
        s = v0 * pow(t, 2) / (2 * t1);
        while (m_animation_curve->len[i] < s) {
            i++;
        }
        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[i]);
    }
    else if ( t < t2 ) {    // t1 <= t < t2, aircraft keeps a constant velocity
        s = v0 * t1 / 2 + v0 * (t - t1);
        while (m_animation_curve->len[i] < s) {
            i++;
        }
        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[i]);
    }
    else if ( t < 1 ) {     // t2 <= t < 1, aircraft is decelerating
        s = 1 + (v0 * pow(t-1, 2)) / (2*(t2-1));
        while (m_animation_curve->len[i] < s && i < m_animation_curve->p.size()-1) {
            i++;
        }
        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[i]);
    }
    else {  // 1 <= t, aircraft stops moving at the original point. For this program we make the aircraft moves only one round
        s = 1.0;
        m_model_mat = glm::mat4(1.0f);
        m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[0]);
    }
}

void Aircraft_Animation::reset()
{
	m_model_mat = glm::mat4(1.0f);

	if (m_animation_curve != nullptr && m_animation_curve->p.size() > 0)
	{
        s = 0.0;
        t = 0.0;
        i = 0;
		m_model_mat = glm::translate(m_model_mat, m_animation_curve->p[0]);
	}
}

