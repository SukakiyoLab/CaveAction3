#pragma once


namespace component {

	class CAT_Component {
	public:
		struct ComponentInitializer {
			virtual ~ComponentInitializer() {};
		};

	public:
		virtual void gain(int delta_time);
		virtual void update();
	};

	


}