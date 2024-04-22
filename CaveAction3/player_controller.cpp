#include "player_controller.h"


namespace component {
	CAT_PlayerController::CAT_PlayerController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D)
    : CAT_CharacterController(new_rigidbody, new_v_controller, new_animator2D){
        
	}

	void CAT_PlayerController::set_input(CAT_Input*const new_input) {
		this->m_input = new_input;
	}

	void CAT_PlayerController::update() {
        int vertical = -(m_input->front - m_input->back);
        int horizontal = m_input->right - m_input->left;

        if (this->state_id == (unsigned short)Move) {
            change_direction(horizontal, vertical);

            if ((m_rigidbody->get_velocity().norm()) > 50) {
                this->m_animator2D->change_animation(1, &(this->direction));
            }
            else {
                this->m_animator2D->change_animation(0, &(this->direction));
            }

            if (m_input->right_trigger == 1) {
                change_state((unsigned short)PlayerState::Attack);
            }

            this->m_virtual_controller->input(Vector3d(horizontal, vertical, 0).normalized());
        }
        else if (this->state_id == (unsigned short)Attack){

            this->m_animator2D->change_animation(2, &(this->direction));

            if (this->state_continuation_time > 450) {
                change_state((unsigned short)PlayerState::Move);
            }

            this->m_virtual_controller->input(Vector3d(0, 0, 0));
            this->m_rigidbody->addForce(-(this->m_rigidbody->get_velocity()) * 20);
            
        }

        
	}


    
}
