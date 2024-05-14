#include "virtual_controller.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "debug_log.h"

#define DEFAULT_MAX_SPEED (1000)

#define DEFAULT_INPUT_FORCE (4500)

using namespace std;

namespace component {


	float CAT_VirtualController::update_element(const float element_dest,
		const float element_present,
		const float delta_time) {
		//実際の入力、仮想の入力の位相を取得//
		const float theta_dest = asin(element_dest);
		const float theta_present = asin(element_present);

		//値の方向を決定する変数//
		int s = 0;

		// 位相の大小で方向を決定//
		if (theta_dest > theta_present) {
			s = 1;
		}
		else if (theta_dest < theta_present) {
			s = -1;
		}

		//仮想の入力を更新//
		return sin(clamp(theta_present + s * this->m_input_speed * delta_time / 1000, (float)(-M_PI / 2.0), (float)(M_PI / 2.0)));
	}

	Vector3d CAT_VirtualController::limit(Vector3d velocity, Vector3d input) {
		if (velocity.norm() > m_max_speed) {
			Vector3d parallel = (velocity.dot(input) / pow(velocity.norm(), 2.0)) * velocity;//inputのvelocityに対して並行成分//

			return input - parallel; //垂直成分//
		}
		else {
			return input;
		}
	}

	//public

	CAT_VirtualController::CAT_VirtualController(CAT_Rigidbody* const rigidbody, ComponentInitializer* const cInit) {
		this->m_rigidbody = rigidbody;
		this->m_input_speed = cInit->input_speed;
		this->m_max_speed = cInit->max_speed;
	}

	void CAT_VirtualController::set_max_speed(const float max_speed) {
		this->m_max_speed = max_speed;
	}

	void CAT_VirtualController::set_periodic_time(const float periodic_time) {
		this->m_periodic_time = periodic_time;
	}

	void CAT_VirtualController::input(const Vector3d input_force) {
		if (input_force != this->m_real_input) {
			this->m_hold_time = 0;
		}
		this->m_real_input = input_force.normalized();
	}

	void CAT_VirtualController::gain(const int delta_time) {
		this->m_hold_time += delta_time;
		
		if (m_rigidbody->get_type() == CAT_Rigidbody::Newton) {

			this->m_virtual_input = Vector3d(
				update_element(this->m_real_input[0], this->m_virtual_input[0], delta_time),
				update_element(this->m_real_input[1], this->m_virtual_input[1], delta_time),
				update_element(this->m_real_input[2], this->m_virtual_input[2], delta_time)
			);
			/*debug::debugLog("new vector: x: %f, y: %f, z: %f\n", this->m_virtual_input[0],
				this->m_virtual_input[1],
				this->m_virtual_input[2]);*/

			Vector3d velocity = this->m_rigidbody->get_velocity();

			/*debug::debugLog("speed: %f\n", velocity.norm());*/


			this->m_rigidbody->addForce(DEFAULT_INPUT_FORCE *  limit(velocity, this->m_virtual_input));
		}
		else if(m_rigidbody->get_type() == CAT_Rigidbody::Aristoteles) {
			
			this->m_rigidbody->set_velocity(150 * this->m_real_input);
			/*debug::debugLog("speed: %f\n", this->m_rigidbody->get_velocity().norm());*/
			
		}

	}

}

