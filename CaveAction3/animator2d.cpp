#include "animator2d.h"

namespace component {

	CAT_Animator2D::CAT_Animator2D(CAT_AnimationRoot* const anim_root, CAT_Animator2D::ComponentInitializer* cInit) {
		this->animation_root = anim_root;

		for (int i = 0; i < cInit->animation_sets->size(); i++) {
			this->save_generate_object((*cInit->animation_sets)[i][0],
				(*cInit->animation_sets)[i][1],
				(*cInit->animation_sets)[i][2]);
		}
	}

	unsigned short CAT_Animator2D::vector2i_to_key(Eigen::Vector2i* vector) {
		unsigned short vector_key = 0;
		
		if (vector != nullptr) {
			if (*vector == Eigen::Vector2i(0, 1)) {
				vector_key = 0;
			}
			else if (*vector == Eigen::Vector2i(-1, 0)) {
				vector_key = 1;
			}
			else if (*vector == Eigen::Vector2i(1, 0)) {
				vector_key = 2;
			}
			else if (*vector == Eigen::Vector2i(0, -1)) {
				vector_key = 3;
			}
		}

		return vector_key;
	}

	void CAT_Animator2D::save_generate_object(unsigned short index, unsigned short direction_key, unsigned short animation_id) {
		if (direction_key < DIRECTION_MAX) {
			//unsigned short direction_key = this->vector2i_to_key(direction);
			
			this->animation_map[std::pair(index, direction_key)] = animation_id;
		}
		else {
			this->animation_map[std::pair(index, 0)] = animation_id;
			this->animation_map[std::pair(index, 1)] = animation_id;
			this->animation_map[std::pair(index, 2)] = animation_id;
			this->animation_map[std::pair(index, 3)] = animation_id;
		}
	}

	void CAT_Animator2D::change_animation(unsigned short index, Eigen::Vector2i* direction) {
		unsigned short direction_key = this->vector2i_to_key(direction);
		this->animation_root->change_animation(this->animation_map[std::pair(index, direction_key)]);
		/*if (direction != nullptr) {
			this->animation_root->change_animation(this->animation_map[std::pair(index, *direction)]);
		}
		else {
			this->animation_root->change_animation(this->animation_map[std::pair(index, Eigen::Vector2i(0, 1))]);
		}*/
	}

}