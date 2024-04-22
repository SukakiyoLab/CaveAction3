#include "CameraObject.h"

namespace object {

	CameraObject::CameraObject(CameraObject::ObjectInitializer objectInit) 
		: GameObject::GameObject(
		{
		objectInit.position,
		objectInit.rotation,
		objectInit.scale
		}) {
		this->m_view_camera = new component::CAT_ViewCamera(this->transform, objectInit.view_port_center, objectInit.view_port_width, objectInit.view_port_height);

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