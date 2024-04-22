#pragma once
#include "GameObject.h"
#include "view_camera.h"
#include "image_projecter.h"

namespace object {

	class CameraObject : GameObject {
	private:
		component::CAT_ViewCamera* m_view_camera;

	public:

		struct ObjectInitializer : GameObject::ObjectInitializer {
			Eigen::Vector3d view_port_center = Eigen::Vector3d(DEFAULT_VIEWPORT_WIDTH / 2, DEFAULT_VIEWPORT_HEIGHT / 2, 0);
			float view_port_width = DEFAULT_VIEWPORT_WIDTH * 1.5;
			float view_port_height = DEFAULT_VIEWPORT_HEIGHT * 1.5;
			ImageProjecter* projecter_ptr = nullptr;
		};

		CameraObject(ObjectInitializer objectInit);
		~CameraObject();

		void SetTarget(component::CAT_Transform* target_ptr);
		void Update() override;

	};

}
