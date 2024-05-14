#include "CameraObject.h"

namespace object {

	CameraObject::CameraObject(CameraObject::ObjectInitializer objectInit) 
		: GameObject::GameObject(static_cast<GameObject::ObjectInitializer>(objectInit)) {
		this->m_view_camera = new component::CAT_ViewCamera(this->transform, 
			dynamic_cast<component::CAT_Transform*>(objectInit.other_obj_components[objectInit.viewCameraInit.other_transform_name][objectInit.viewCameraInit.other_transform_id]), 
			&(objectInit.viewCameraInit));

		objectInit.projecter_ptr->set_camera(this->m_view_camera);

	}

	CameraObject::~CameraObject() {
		delete this->m_view_camera;
	}


	void CameraObject::SetTarget(component::CAT_Transform* target_ptr) {
		this->m_view_camera->set_target(target_ptr);
	};

	void CameraObject::Update() {
		this->m_view_camera->update();
	}


}