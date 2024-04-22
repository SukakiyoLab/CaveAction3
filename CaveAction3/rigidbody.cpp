#include "rigidbody.h"
#include <cstdio>

#include "debug_log.h"

using namespace Eigen;

namespace component {

	CAT_Rigidbody::CAT_Rigidbody(CAT_Transform* const transform,
		Type type,
		const float mass,
		const float coefficient,
		const Vector3d virtual_normal_force) {

		this->type = type;
		this->mass = mass;
		this->friction_coefficient = coefficient;

		this->m_transform = transform;
		this->m_virtual_normal_force = virtual_normal_force;
	}


	void CAT_Rigidbody::gain(float delta_time) {
		Vector3d pre_pos = this->m_transform->get_position();
		Vector3d next_pos = pre_pos + (this->m_velocity) * delta_time / 1000;
		next_pos = (next_pos).array().round().matrix();
		this->m_transform->set_position(next_pos);


		// 速度を計算//
		if (type == Newton) {
			Vector3d pre_velocity = this->m_velocity;
			Vector3d friction_force = -this->friction_coefficient * this->m_virtual_normal_force.norm() * this->m_velocity.normalized();

			Vector3d next_velocity;

			

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
					if (force_h.norm() > this->static_friction_magnitude){
						h_check = 1;
					}
					next_velocity = pre_velocity + (v_check * force_v + h_check * force_h + friction_force) * delta_time / 1000 / this->mass;
				}
				else {
					next_velocity = pre_velocity + (this->m_sum_force + friction_force) * delta_time / 1000 / this->mass;
				}
			
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

}

