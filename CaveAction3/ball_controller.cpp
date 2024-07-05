#include "ball_controller.h"

#include <functional>

namespace component {

	void CAT_BallController::set_direction(Eigen::Vector2i new_direction) { 
		this->direction = new_direction; 

		if (this->direction[0] == 1) {
			this->transform_ptr->set_rotation({ 0,0,-90 });
		}
		else if (this->direction[0] == -1) {
			this->transform_ptr->set_rotation({ 0,0,90 });
		}
		else if (this->direction[1] == 1) {
			this->transform_ptr->set_rotation({ 0,0,0 });
		}
		else if (this->direction[1] == -1) {
			this->transform_ptr->set_rotation({0,0,180});
		}

	}

	CAT_BallController::CAT_BallController(
		CAT_Transform* const transform_ptr, 
		CAT_AnimationImage* const animation_image_ptr, 
		CAT_Rigidbody* const rigidbody_ptr, 
		CAT_AnimationCurve* const animation_curve_ptr,
		ComponentInitializer* const cInit) {
		this->transform_ptr = transform_ptr;
		
		this->animation_image_ptr = animation_image_ptr;
		
		this->rigidbody_ptr = rigidbody_ptr;
		
		this->animation_curve_ptr = animation_curve_ptr;
		
		this->animation_curve_ptr->regist_address(this->rigidbody_ptr->get_speed_ptr());
		
		set_direction(cInit->direction);
	}

	void CAT_BallController::update() {
		this->rigidbody_ptr->set_velocity(Eigen::Vector3d{ (double)this->direction[0], (double)this->direction[1], 0 });
	}


}