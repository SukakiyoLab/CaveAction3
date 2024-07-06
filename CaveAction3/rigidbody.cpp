#include "rigidbody.h"
#include <cstdio>

#include "debug_log.h"

using namespace Eigen;

namespace component {

	CAT_Rigidbody::CAT_Rigidbody(CAT_Transform* const transform, CAT_Rigidbody::ComponentInitializer* cInit) {

		this->type = cInit->type;
		this->mass = cInit->mass;
		this->friction_coefficient = cInit->coefficient;

		this->m_transform = transform;
		this->m_virtual_normal_force = cInit->virtual_normal_force;
	}


	void CAT_Rigidbody::gain(int delta_time) {
		this->m_delta_time = delta_time;
		
	}

	void CAT_Rigidbody::update() {
		
		Vector3d next_pos = this->m_transform->get_position() + (this->m_velocity) * m_delta_time / 1000;
		next_pos = (next_pos).array().round().matrix();
		this->m_transform->set_position(next_pos);

		//debug::print("velocity", this->m_velocity);
		//debug::debugLog("[speed] : %f\n", this->m_speed);

		// 速度を計算//
		if (type == Newton) {
			Vector3d pre_velocity = this->m_velocity;
			Vector3d friction_force = -this->friction_coefficient * this->m_virtual_normal_force.norm() * this->m_velocity.normalized();

			Vector3d next_velocity;

			/*

			//debug::print("sum_force", this->m_sum_force);
			Vector3d force_v;
			Vector3d force_h;
			if (pre_velocity.norm() > DEFAULT_STATIC_VELOCITY) {

				Vector3d force = this->m_sum_force;
				float force_v_magnitude = pre_velocity.dot(force) / pre_velocity.norm();
				force_v = (pre_velocity / pre_velocity.norm()) * force_v_magnitude;
				force_h = force - force_v;
			}
			else {
				force_v = this->m_sum_force;
				force_h = Vector3d::Zero();
			}

			//debug::print("force_v", force_v);
			//debug::print("force_h", force_h);

			if (pre_velocity.norm() < DEFAULT_STATIC_VELOCITY) {

				int v_check = 0;
				int h_check = 0;

				if (force_v.norm() > this->static_friction_magnitude) {
					v_check = 1;
				}
				if (force_h.norm() > this->static_friction_magnitude) {
					h_check = 1;
				}
				next_velocity = pre_velocity + (v_check * force_v + h_check * force_h + friction_force) * m_delta_time / 1000 / this->mass;
			}
			else {
				next_velocity = pre_velocity + (this->m_sum_force + friction_force) * m_delta_time / 1000 / this->mass;
			}
			*/

			next_velocity = pre_velocity + (this->m_sum_force + friction_force) * m_delta_time / 1000 / this->mass;

			//next_velocity = pre_velocity + (this->m_sum_force + friction_force) * delta_time / 1000 / this->mass;

			this->m_velocity = next_velocity;

			this->m_sum_force = Vector3d::Zero();
		}
	}

	int CAT_Rigidbody::addForce(const Vector3d force) {
		if (type == Newton) {
			this->m_sum_force += force;
			return 0;
		}
		else {
			debug::debugLog("Type of Rigidbody is different!\n");
			return -1;
		}
	}


	int CAT_Rigidbody::set_velocity(const Vector3d velocity) {

		if (type == Aristoteles) {
			this->m_velocity = velocity;
			if (this->m_speed > 0) {
				this->m_velocity = (this->m_velocity / this->m_velocity.norm()) * this->m_speed;
			}
			return 0;
		}
		else {
			debug::debugLog("Type of Rigidbody is different!\n");
			return -1;
		}
	}

	CAT_Rigidbody::Type CAT_Rigidbody::get_type() {
		return this->type;
	}

	void CAT_Rigidbody::reset_generator() {
		this->m_velocity = Eigen::Vector3d::Zero();
		this->m_transform->set_position(pre_pos);
	}

}

