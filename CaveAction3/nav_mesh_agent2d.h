#pragma once

#include "shared_struct.h"
#include "transform.h"
#include "nav_mesh_system2d.h"

namespace component {

	class CAT_NavMeshAgent {
	private:
		unsigned short current_id = 0;
		unsigned short destination_id = 0;
		unsigned short next_id = 0;

		CAT_NMRect* current_rect_ptr = nullptr;
		CAT_NMRect* next_rect_ptr = nullptr;

		CAT_Transform* self_transform_ptr;
		CAT_Transform* target_transform_ptr;

		NavMeshSystem2D* nmsys_ptr;

	private:
		Eigen::Vector2i vector_3d_to_2i(Eigen::Vector3d vec);

	public:
		CAT_NavMeshAgent(CAT_Transform* init_self, CAT_Transform* init_target, NavMeshSystem2D* init_nmsys_ptr);

		void calculate();

		bool check();

		Eigen::Vector3d get_direction();

		std::pair<unsigned short, unsigned short> get_id_pair();
	};
}
