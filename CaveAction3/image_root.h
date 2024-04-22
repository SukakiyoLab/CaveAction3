#pragma once

#include "transform.h"
#include "view_camera.h"

namespace component {

	class CAT_ImageRoot {
	protected:
		CAT_Transform* m_transform;
	public:
		CAT_ImageRoot(CAT_Transform* const transform);
		virtual void project(CAT_ViewCamera* camera);

		double get_position_height();

		static bool cmp(CAT_ImageRoot* a, CAT_ImageRoot* b);
	};
}