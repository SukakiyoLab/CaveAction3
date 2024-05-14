#pragma once
#include "GameObject.h"
#include "view_camera.h"

namespace object {

	class CameraObject : public GameObject {
	private:
		component::CAT_ViewCamera* m_view_camera;

	public:

		struct ObjectInitializer : public GameObject::ObjectInitializer {
			component::CAT_ViewCamera::ComponentInitializer viewCameraInit;
		};

		CameraObject(ObjectInitializer objectInit);
		~CameraObject();

		void SetTarget(component::CAT_Transform* target_ptr);
		void Update() override;

	};

}
