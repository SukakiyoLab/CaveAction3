#include "Player2D.h"


namespace object {
	Player2D::Player2D(Player2D::ObjectInitializer objectInit) 
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


		this->player_controller = new component::CAT_PlayerController(this->rigidbody, this->virtual_contoroller, this->animator_2d);
		this->player_controller->set_input(objectInit.player_input);

		this->type = Type::Player;

	}

	Player2D::~Player2D() {
		delete this->player_controller;
	}

	void Player2D::Update() {
		this->player_controller->update();

		AnimationEntity2D::Update();
	}

	void Player2D::Gain(double delta_time) {
		this->player_controller->gain(delta_time);

		AnimationEntity2D::Gain(delta_time);
	}

}

