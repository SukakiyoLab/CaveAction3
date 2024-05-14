#include "view_camera.h"

namespace component {

	CAT_ViewCamera::CAT_ViewCamera(CAT_Transform* camera_transform, CAT_Transform* target_transform, ComponentInitializer* cInit) {
		//this->m_target = nullptr;
		this->m_camera_transform = camera_transform;
		set_target(target_transform);
		this->m_width = cInit->width;
		this->m_height = cInit->height;
		this->m_view_port_center = cInit->view_port_center;

		
		update();

		if (this->m_target == nullptr) {
			this->m_camera_transform->set_position(Eigen::Vector3d(DEFAULT_VIEWPORT_WIDTH / 2, DEFAULT_VIEWPORT_HEIGHT / 2, 0));
		}
	}

	void CAT_ViewCamera::update() {
		this->m_view_port.top = -float(m_height / 2);
		this->m_view_port.left = -float(m_width / 2);
		this->m_view_port.right = m_width / 2;
		this->m_view_port.bottom = m_height / 2;

		/* ‚±‚±‚ÍŠeŽ©•ÏX */
		if (this->m_target != nullptr) {
			this->m_camera_transform->set_position(this->m_target->get_position());
		}
	}

	void CAT_ViewCamera::set_target(CAT_Transform* target) {
		this->m_target = target;
	}

	Eigen::Vector3d CAT_ViewCamera::get_position() {
		return this->m_camera_transform->get_position();
	}

	Eigen::Vector3d CAT_ViewCamera::get_view_port_center() {
		return this->m_view_port_center;
	}

	float CAT_ViewCamera::get_view_port_param(unsigned char direction_num) {

		switch (direction_num) {
		case 0:
			return this->m_view_port.top;
			break;
		case 1:
			return this->m_view_port.left;
			break;
		case 2:
			return this->m_view_port.right;
			break;
		case 3:
			return this->m_view_port.bottom;
			break;

		}

	}




}