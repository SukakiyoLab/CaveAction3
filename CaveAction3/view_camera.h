#pragma once
#include "transform.h"

#define DEFAULT_VIEWPORT_WIDTH (640)
#define DEFAULT_VIEWPORT_HEIGHT (480)


namespace component {

	class CAT_ViewCamera {
	private:
		CAT_Transform* m_target;
		CAT_Transform* m_camera_transform;
		unsigned int m_width;
		unsigned int m_height;

		Eigen::Vector3d m_view_port_center;

	public:
		struct CAT_ViewPort {
			float top;
			float left;
			float right;
			float bottom;
		};

	private:
		CAT_ViewPort m_view_port;
 
	public:
		CAT_ViewCamera(CAT_Transform* camera_transform, Eigen::Vector3d view_port_center, unsigned int width=(DEFAULT_VIEWPORT_WIDTH*1.5), unsigned int height=(DEFAULT_VIEWPORT_HEIGHT*1.5));
		
		void update();

		void set_target(CAT_Transform* target);

		Eigen::Vector3d get_position();
		Eigen::Vector3d get_view_port_center();
		float get_view_port_param(unsigned char direction_num);
	};

}
