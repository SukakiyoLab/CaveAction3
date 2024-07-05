#include "player_controller.h"


namespace component {
	CAT_PlayerController::CAT_PlayerController(
        CAT_Transform* const new_transform,
        CAT_Rigidbody* const new_rigidbody,
        CAT_VirtualController* const new_v_controller,
        CAT_Animator2D* const new_animator2D,
        game_system_3::ObjectGenerator* generator_ptr,
        FunctionMap* func_map_ptr,
        ComponentInitializer* cInit
    )
    : CAT_CharacterController(new_rigidbody, new_v_controller, new_animator2D, static_cast<CAT_CharacterController::ComponentInitializer*>(cInit)){
        this->transform_ptr = new_transform;
        this->generator_ptr = generator_ptr;
        this->func_map_ptr = func_map_ptr;
        set_input(cInit->player_input_ptr);

        ballData = this->func_map_ptr->use_char_func("fireball_object_data");
        ballPositionData = ballData->nexts["transform"][0]->nexts["position"][0];
        for (auto item : ballData->nexts["component"]) {
            if (item->nexts.count("ball_controller") > 0) {
                ballDirectionData = item->nexts["ball_controller"][0]->nexts["direction"][0];
            }
        }

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
                change_state((unsigned short)PlayerState::Magic);
                //change_state((unsigned short)PlayerState::Attack);

                Eigen::Vector3d generate_pos = this->transform_ptr->get_position() + Eigen::Vector3d(this->direction[0], this->direction[1], 0) * OFFSET;

                ballPositionData->nexts["x"][0]->item = std::to_string(generate_pos[0]);
                ballPositionData->nexts["y"][0]->item = std::to_string(generate_pos[1]);
                ballDirectionData->nexts["x"][0]->item = std::to_string(this->direction[0]);
                ballDirectionData->nexts["y"][0]->item = std::to_string(this->direction[1]);
                
                generator_ptr->save_generate_object(ballData);

            }

            this->m_virtual_controller->input(Vector3d(horizontal, vertical, 0).normalized());
        }
        else if (this->state_id == (unsigned short)Magic) {
            if ((m_rigidbody->get_velocity().norm()) > 50) {
                this->m_animator2D->change_animation(4, &(this->direction));
            }
            else {
                this->m_animator2D->change_animation(3, &(this->direction));
            }

            if (this->state_continuation_time > 500) {
                change_state((unsigned short)PlayerState::Move);
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
