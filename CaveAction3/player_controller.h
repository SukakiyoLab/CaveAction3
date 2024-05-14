#pragma once

#include "character_controller.h"

#include <eigen3/Eigen/Core>
#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY


namespace component {

	class CAT_PlayerController : public CAT_CharacterController {
	private:
		CAT_Input* m_input = nullptr;

		enum PlayerState {
			Move,
			Attack
		};

		PlayerState state = PlayerState::Move;

	public:
		struct ComponentInitializer : public CAT_CharacterController::ComponentInitializer {
			CAT_Input* player_input_ptr;
		};


	public:
		CAT_PlayerController(CAT_Rigidbody*const new_rigidbody,CAT_VirtualController*const new_v_controller,CAT_Animator2D*const new_animator2D, ComponentInitializer* cInit);
		void set_input(CAT_Input* const new_input);
		void update() override;
	
	};
}