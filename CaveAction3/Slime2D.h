#pragma once

#include "AnimationEntity2D.h"

#include "nav_mesh_agent2d.h"

#include "slime_controller.h"

#include "nav_mesh_system2d.h"

namespace object {
	class Slime2D : public AnimationEntity2D {
	private:
		component::CAT_NavMeshAgent2D* nm_agent_ptr;

		component::CAT_SlimeController* slime_controller;

	public:

		struct ObjectInitializer : AnimationEntity2D::ObjectInitializer {
			//CAT_Input* player_input;
			//component::CAT_Transform* target_trans_ptr;
			//NavMeshSystem2D* nmsys_ptr;
			//std::string target_obj_name;
			component::CAT_NavMeshAgent2D::ComponentInitializer navMeshAgent2DInit;

			component::CAT_SlimeController::ComponentInitializer slimeInit;
		};

		Slime2D(ObjectInitializer objectInit);
		~Slime2D();

		void Update() override;
		void Gain(int delta_time) override;

	};


}

