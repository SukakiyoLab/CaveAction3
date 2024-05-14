#pragma once

#include "GameObject.h"

#include "animation_image.h"
#include "animator2d.h"
#include "rigidbody.h"
#include "boxcollider2d.h"
#include "virtual_controller.h"
#include "image.h"

#include <vector>
#include <tuple>

#include "image_projecter.h"
#include "collider_manager.h"

namespace object {
	class AnimationEntity2D : public GameObject {
	protected:
		component::CAT_AnimationImage* animation_image;
		component::CAT_Animator2D* animator_2d;
		component::CAT_Rigidbody* rigidbody;
		component::CAT_BoxCollider2D* box_collider;
		component::CAT_VirtualController* virtual_contoroller;

#ifdef _DEBUG
		component::CAT_Image* debug_image;
#endif

	public:
		struct ObjectInitializer : GameObject::ObjectInitializer{
			
			component::CAT_AnimationImage::ComponentInitializer animImageInit;

			//std::vector<std::tuple<unsigned short, unsigned short, unsigned short>> animation_sets = std::vector<std::tuple<unsigned short, unsigned short, unsigned short>>();

			component::CAT_Animator2D::ComponentInitializer animator2DInit;

			component::CAT_Rigidbody::ComponentInitializer rigidbodyInit;

			//component::CAT_Rigidbody::Type physics_type = component::CAT_Rigidbody::Type::Newton;
			//float mass = 1.0f;

			component::CAT_VirtualController::ComponentInitializer virtualControllerInit;

			//float input_speed = 10.0f;
			//float max_speed = 100.0f;

			component::CAT_BoxCollider2D::ComponentInitializer boxColliderInit;

			//unsigned short collider_layer = 0;
			//float collider_w = 1.0f;
			//float collider_h = 1.0f;
			//Vector2d collider_offset = Vector2d::Zero();
			//ColliderManager* collider_manager = nullptr;

		};

		AnimationEntity2D(AnimationEntity2D::ObjectInitializer objectInit);
		virtual ~AnimationEntity2D();

		virtual void Update() override;
		virtual void Gain(int delta_time) override;

	};

}