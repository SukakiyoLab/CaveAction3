#include "Slime2D.h"


namespace object {
	Slime2D::Slime2D(Slime2D::ObjectInitializer objectInit)
		: AnimationEntity2D::AnimationEntity2D({
			objectInit.position,
			objectInit.rotation,
			objectInit.scale,
			objectInit.animation_data,
			objectInit.renderer,
			objectInit.image_offset,
			objectInit.image_layer,
			objectInit.projecter,

			objectInit.animation_sets,

			objectInit.physics_type,
			objectInit.mass,

			objectInit.input_speed,
			objectInit.max_speed,

			objectInit.collider_layer,
			objectInit.collider_w,
			objectInit.collider_h,
			objectInit.collider_offset,
			objectInit.collider_manager
			}) {

		this->nm_agent_ptr = new component::CAT_NavMeshAgent(this->transform, objectInit.target_trans_ptr, objectInit.nmsys_ptr);

		this->slime_controller = new component::CAT_SlimeController(this->rigidbody, this->virtual_contoroller, this->animator_2d, this->nm_agent_ptr);
		//this->slime_controller->set_input(objectInit.player_input);

		this->type = Type::Enemy;

	}

	Slime2D::~Slime2D() {
		delete this->slime_controller;
	}

	void Slime2D::Update() {
		this->slime_controller->update();

		AnimationEntity2D::Update();
	}

	void Slime2D::Gain(double delta_time) {
		this->slime_controller->gain(delta_time);

		AnimationEntity2D::Gain(delta_time);
	}

}

