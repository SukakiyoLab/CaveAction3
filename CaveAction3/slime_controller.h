#pragma once

#include "character_controller.h"
#include "nav_mesh_agent2d.h"

#include "xml_data.h"
#include "function_map.h"

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

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->rigidbody_id = std::stoi(xmldata_ptr->nexts["rigidbody_id"][0]->item);
			cInit_ptr->virtual_controller_id = std::stoi(xmldata_ptr->nexts["virtual_controller_id"][0]->item);
			cInit_ptr->animator2D_id = std::stoi(xmldata_ptr->nexts["animator2D_id"][0]->item);
			cInit_ptr->nav_mesh_agent_id = std::stoi(xmldata_ptr->nexts["nav_mesh_agent_id"][0]->item);

			return cInit_ptr;
		}

	public:
		CAT_SlimeController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D, CAT_NavMeshAgent2D* new_nm_agent, ComponentInitializer* cInit);
		void update() override;
	};
}