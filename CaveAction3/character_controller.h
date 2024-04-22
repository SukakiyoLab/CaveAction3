#pragma once

#include <eigen3/Eigen/Core>
#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#include "virtual_controller.h"
#include "rigidbody.h"
#include "animator2d.h"

#include "shared_struct.h"

namespace component {

	class CAT_CharacterController {
	protected:
		
		CAT_Rigidbody* m_rigidbody;
		CAT_VirtualController* m_virtual_controller;

		CAT_Animator2D* m_animator2D;

		

		unsigned short state_id = 0; /* 新しいステート！ */

		Eigen::Vector2i direction = Eigen::Vector2i(0, 1);


		double state_continuation_time = 0;

	protected:
		void change_state(unsigned short new_state_id);


	public:
		CAT_CharacterController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D);
		virtual void update();
		void gain(double delta_time);

		void change_direction(short horizontal, short vertical);


	};


}
