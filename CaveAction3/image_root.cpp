#include "image_root.h"

#include "debug_log.h"

namespace component {

	CAT_ImageRoot::CAT_ImageRoot(CAT_Transform* const transform, CAT_ImageRoot::ComponentInitializer* cInit) {
		this->m_transform = transform;
	};

	void CAT_ImageRoot::project(CAT_ViewCamera* camera) {};

	double CAT_ImageRoot::get_position_height() {
		debug::debugLog("%d\n", this->m_transform->get_position()[0]);
		return this->m_transform->get_position()[1];
	}

	bool CAT_ImageRoot::cmp(CAT_ImageRoot** a, CAT_ImageRoot** b) {

		if (a != nullptr && b != nullptr) {

			if ((*a) == nullptr) {
				return true;
			}

			if ((*b) == nullptr) {
				return false;
			}

			return ((**a).get_position_height() < (**b).get_position_height());
		}
		else {
			return false;
		}
	}
}