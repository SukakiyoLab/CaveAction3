#include "slime_controller.h"


namespace component {

	CAT_SlimeController::CAT_SlimeController(CAT_Rigidbody* const new_rigidbody, CAT_VirtualController* const new_v_controller, CAT_Animator2D* const new_animator2D, CAT_NavMeshAgent* new_nm_agent)
	:CAT_CharacterController(new_rigidbody, new_v_controller, new_animator2D) {
        this->nm_agent_ptr = new_nm_agent;
	}

    void CAT_SlimeController::update() {

        int vertical = 1;
        int horizontal = 0;

        /*if (this->nm_agent_ptr->check()) {
            
        }*/
        this->nm_agent_ptr->calculate();
        
        debug::debugLog("%d %d\n", this->nm_agent_ptr->get_id_pair().first, this->nm_agent_ptr->get_id_pair().second);
        

        Vector3d double_direction = (this->nm_agent_ptr->get_direction()).normalized();

        if (double_direction[1] < -0.3) {
            horizontal = 0;
            vertical = -1;
        }
        else if (double_direction[1] > 0.3) {
            horizontal = 0;
            vertical = 1;
        }
        else if (double_direction[0] < 0) {
            horizontal = -1;
            vertical = 0;
        }
        else if (double_direction[0] > 0) {
            horizontal = 1;
            vertical = 0;
        }


        if (this->state_id == (unsigned short)Move) {
            change_direction(horizontal, vertical);

            if ((m_rigidbody->get_velocity().norm()) > 50) {
                this->m_animator2D->change_animation(1, &(this->direction));
            }
            else {
                this->m_animator2D->change_animation(0, &(this->direction));
            }

            /*if (m_input->right_trigger == 1) {
                change_state((unsigned short)Attack);
            }*/

            this->m_virtual_controller->input(double_direction);
        }
        else if (this->state_id == (unsigned short)Attack) {

            this->m_animator2D->change_animation(2, &(this->direction));

            if (this->state_continuation_time > 450) {
                change_state((unsigned short)SlimeState::Move);
            }

            this->m_virtual_controller->input(Vector3d(0, 0, 0));
            this->m_rigidbody->addForce(-(this->m_rigidbody->get_velocity()) * 20);

        }


    }


}



