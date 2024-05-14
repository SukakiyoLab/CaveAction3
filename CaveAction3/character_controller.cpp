#include "character_controller.h"


namespace component {

	CAT_CharacterController::CAT_CharacterController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D, CAT_CharacterController::ComponentInitializer* cInit) {
		this->m_rigidbody = new_rigidbody;
		this->m_virtual_controller = new_v_controller;
		this->m_animator2D = new_animator2D;
	}

    void CAT_CharacterController::gain(int delta_time) {
        this->state_continuation_time += delta_time;
    }


	void CAT_CharacterController::update() {

	}

	void CAT_CharacterController::change_direction(short horizontal, short vertical) {
        if (vertical == 1) {
            this->direction = Vector2i(0, 1);
        }
        else if (vertical == -1) {
            this->direction = Vector2i(0, -1);
        }
        else if (horizontal == 1) {
            this->direction = Vector2i(1, 0);
        }
        else if (horizontal == -1) {
            this->direction = Vector2i(-1, 0);
        }
	}

    void CAT_CharacterController::change_state(unsigned short new_state_id) {
        this->state_id = new_state_id;
        this->state_continuation_time = 0;
    }

}