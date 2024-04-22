#include "nav_mesh_agent2d.h"


namespace component {

	Eigen::Vector2i CAT_NavMeshAgent::vector_3d_to_2i(Eigen::Vector3d vec) {
		return Eigen::Vector2i((int)vec[0], (int)vec[1]);
	}

	CAT_NavMeshAgent::CAT_NavMeshAgent(CAT_Transform* init_self, CAT_Transform* init_target, NavMeshSystem2D* init_nmsys_ptr) {
		this->self_transform_ptr = init_self;
		this->target_transform_ptr = init_target;
		this->nmsys_ptr = init_nmsys_ptr;

		calculate();

	}

	void CAT_NavMeshAgent::calculate() {
		this->current_id = this->nmsys_ptr->get_id(vector_3d_to_2i(this->self_transform_ptr->get_position()));
		this->destination_id = this->nmsys_ptr->get_id(vector_3d_to_2i(this->target_transform_ptr->get_position()));

		
		this->current_rect_ptr = this->nmsys_ptr->get_rect(this->current_id);
		this->next_rect_ptr = this->nmsys_ptr->get_rect(this->nmsys_ptr->get_next_info(this->current_id, this->destination_id).first);

		
	}

	bool CAT_NavMeshAgent::check() {
		Eigen::Vector2i current_pos_2d = vector_3d_to_2i(this->self_transform_ptr->get_position());

		if (current_pos_2d[0] < current_rect_ptr->left) {
			return true;
		}
		else if (current_pos_2d[0] > current_rect_ptr->right) {
			return true;
		}
		else if (current_pos_2d[1] < current_rect_ptr->top) {
			return true;
		}
		else if (current_pos_2d[1] > current_rect_ptr->bottom) {
			return true;
		}



		return false;
	}

	Eigen::Vector3d CAT_NavMeshAgent::get_direction() {

		if (this->nmsys_ptr->get_next_info(this->current_id, this->destination_id).second > 1) {
			return Eigen::Vector3d(next_rect_ptr->center[0], next_rect_ptr->center[1], 0) - this->self_transform_ptr->get_position();
		}
		else {
			return this->target_transform_ptr->get_position() - this->self_transform_ptr->get_position();
		}
	}

	std::pair<unsigned short, unsigned short> CAT_NavMeshAgent::get_id_pair() {
		return this->nmsys_ptr->get_next_info(this->current_id, this->destination_id);
	}
}