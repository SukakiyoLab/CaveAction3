#pragma once

#include "character_controller.h"
#include "nav_mesh_agent2d.h"

namespace component {

	class CAT_SlimeController : public CAT_CharacterController{
	private:
	

		enum SlimeState {
			Move,
			Attack
		};

		SlimeState state = SlimeState::Move;

		CAT_NavMeshAgent* nm_agent_ptr;


	public:
		CAT_SlimeController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D, CAT_NavMeshAgent* new_nm_agent);
		void update() override;
	};
}