#pragma once

namespace component {

	// こいつだけ特殊
	class CAT_AnimationRoot {
	protected:
		unsigned short state_id = 0;

	public:
		virtual int change_animation(unsigned short new_id);

	};
}