#include "AnimationEntity2D.h"

namespace object {
	AnimationEntity2D::AnimationEntity2D(AnimationEntity2D::ObjectInitializer objectInit) 
	: GameObject::GameObject(static_cast<GameObject::ObjectInitializer>(objectInit)) {
		this->animation_image = new component::CAT_AnimationImage(this->transform, &(objectInit.animImageInit), objectInit.renderer_ptr);
		
		objectInit.projecter_ptr->save(this->animation_image, objectInit.animImageInit.image_layer);

		this->animator_2d = new component::CAT_Animator2D(this->animation_image, &(objectInit.animator2DInit));

		

		this->rigidbody = new component::CAT_Rigidbody(this->transform, &(objectInit.rigidbodyInit));

		this->virtual_contoroller = new component::CAT_VirtualController(this->rigidbody, &(objectInit.virtualControllerInit));
		//this->virtual_contoroller->set_max_speed(objectInit.max_speed);

		this->box_collider = new component::CAT_BoxCollider2D(this->transform,
			this->rigidbody,
			&(objectInit.boxColliderInit));

		objectInit.collider_manager_ptr->save(this->box_collider, objectInit.boxColliderInit.layer);

#ifdef _DEBUG
		component::CAT_BoxCollider2D::Range range = this->box_collider->get_range();
		component::CAT_Image::ComponentInitializer debugImageInit;
		debugImageInit.path = "./resource/imgs/panel1.png";
		debugImageInit.width = range.width / 32;
		debugImageInit.height = range.height / 32;
		debugImageInit.offset = range.offset.cast<int>();
		debugImageInit.image_alpha = 100;
		debugImageInit.image_layer = 8;
		this->debug_image = new component::CAT_Image(this->transform, &debugImageInit, objectInit.renderer_ptr);
		objectInit.projecter_ptr->save(this->debug_image, debugImageInit.image_layer);
#endif

	}


	AnimationEntity2D::~AnimationEntity2D(){

		delete this->animation_image;
		delete this->animator_2d;
		delete this->rigidbody;
		delete this->virtual_contoroller;
		delete this->box_collider;

	}

	void AnimationEntity2D::Update() {
		//this->virtual_contoroller->input(Eigen::Vector3d(1, 0, 0));
	}

	void AnimationEntity2D::Gain(int delta_time) {
		

		this->animation_image->gain(delta_time);

		this->virtual_contoroller->gain(delta_time);
		this->rigidbody->gain(delta_time);
	}
}