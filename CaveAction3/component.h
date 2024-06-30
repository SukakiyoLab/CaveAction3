#pragma once


namespace component {

	class CAT_Component {
	public:
		struct ComponentInitializer {
			inline virtual ~ComponentInitializer() {};
		};

	public:
		virtual ~CAT_Component();
		virtual void gain(int delta_time);
		virtual void update();
	};

	


}