#include "image_root.h"

namespace component {

	CAT_ImageRoot::CAT_ImageRoot(CAT_Transform* const transform) {
		this->m_transform = transform;
	};

	void CAT_ImageRoot::project(CAT_ViewCamera* camera) {};

	double CAT_ImageRoot::get_position_height() {
		return this->m_transform->get_position()[1];
	}

	bool CAT_ImageRoot::cmp(CAT_ImageRoot* a, CAT_ImageRoot* b) {

		if (a != nullptr && b != nullptr) {

			return a->get_position_height() < b->get_position_height();
		}
		else {
			return false;
		}
	}
}