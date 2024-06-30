#include "nav_mesh_agent2d.h"




namespace component {

	Eigen::Vector2i CAT_NavMeshAgent2D::vector_3d_to_2i(Eigen::Vector3d vec) {
		return Eigen::Vector2i((int)vec[0], (int)vec[1]);
	}

	CAT_NavMeshAgent2D::CAT_NavMeshAgent2D(CAT_Transform* init_self, CAT_Transform* init_target, NavMeshSystem2D* init_nmsys_ptr) {
		this->self_transform_ptr = init_self;
		this->target_transform_ptr = init_target;
		this->nmsys_ptr = init_nmsys_ptr;

		calculate();

	}

	void CAT_NavMeshAgent2D::calculate() {
		this->current_id = this->nmsys_ptr->get_id(vector_3d_to_2i(this->self_transform_ptr->get_position()));
		this->destination_id = this->nmsys_ptr->get_id(vector_3d_to_2i(this->target_transform_ptr->get_position()));

		debug::debugLog("Current : %d Destination : %d\n", this->current_id, this->destination_id);
		
		this->current_rect_ptr = this->nmsys_ptr->get_rect(this->current_id);
		this->next_rect_ptr = this->nmsys_ptr->get_rect(this->nmsys_ptr->get_next_info(this->current_id, this->destination_id).first);

		
	}

	bool CAT_NavMeshAgent2D::check() {
		Eigen::Vector2i current_pos_2d = vector_3d_to_2i(this->self_transform_ptr->get_position());

		if (current_pos_2d[0] < current_rect_ptr->left - NAVMESH_OFFSET) {
			return true;
		}
		else if (current_pos_2d[0] > current_rect_ptr->right + NAVMESH_OFFSET) {
			return true;
		}
		else if (current_pos_2d[1] < current_rect_ptr->top - NAVMESH_OFFSET) {
			return true;
		}
		else if (current_pos_2d[1] > current_rect_ptr->bottom + NAVMESH_OFFSET) {
			return true;
		}



		return false;
	}

	Eigen::Vector3d CAT_NavMeshAgent2D::get_direction() {

		Eigen::Vector3d simple_direction = this->target_transform_ptr->get_position() - this->self_transform_ptr->get_position();

		if (this->nmsys_ptr->get_next_info(this->current_id, this->destination_id).second > 2) {
			Eigen::Vector2i rect_direction = next_rect_ptr->center - current_rect_ptr->center;
			return Eigen::Vector3d(rect_direction[0], rect_direction[1], 0);
		}
		else {
			return simple_direction;
		}
	}

	std::pair<unsigned short, unsigned short> CAT_NavMeshAgent2D::get_id_pair() {
		return this->nmsys_ptr->get_next_info(this->current_id, this->destination_id);
	}
}