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

		CAT_NavMeshAgent2D* nm_agent_ptr;

	public:
		struct ComponentInitializer : public CAT_CharacterController::ComponentInitializer {
			unsigned short nav_mesh_agent_id = 0;
		};

	public:
		CAT_SlimeController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D, CAT_NavMeshAgent2D* new_nm_agent, ComponentInitializer* cInit);
		void update() override;
	};
}