#pragma once

#include "character_controller.h"

#include <eigen3/Eigen/Core>
#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#include "xml_data.h"
#include "function_map.h"


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

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr, CAT_Input* input_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->rigidbody_id = std::stoi(xmldata_ptr->nexts["rigidbody_id"][0]->item);
			cInit_ptr->virtual_controller_id = std::stoi(xmldata_ptr->nexts["virtual_controller_id"][0]->item);
			cInit_ptr->animator2D_id = std::stoi(xmldata_ptr->nexts["animator2D_id"][0]->item);
			cInit_ptr->player_input_ptr = input_ptr;

			return cInit_ptr;
		}


	public:
		CAT_PlayerController(CAT_Rigidbody*const new_rigidbody,CAT_VirtualController*const new_v_controller,CAT_Animator2D*const new_animator2D, ComponentInitializer* cInit);
		void set_input(CAT_Input* const new_input);
		void update() override;
	
	};
}