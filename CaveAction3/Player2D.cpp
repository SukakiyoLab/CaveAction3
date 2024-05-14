#include "Player2D.h"


namespace object {
	Player2D::Player2D(Player2D::ObjectInitializer objectInit) 
		: AnimationEntity2D::AnimationEntity2D(static_cast<AnimationEntity2D::ObjectInitializer>(objectInit)) {


		this->player_controller = new component::CAT_PlayerController(this->rigidbody, this->virtual_contoroller, this->animator_2d, &(objectInit.playeyControllerInit));
		this->player_controller->set_input(objectInit.playeyControllerInit.player_input_ptr);

		this->type = Type::Player;

	}

	Player2D::~Player2D() {
		delete this->player_controller;
	}

	void Player2D::Update() {
		this->player_controller->update();

		AnimationEntity2D::Update();
	}

	void Player2D::Gain(int delta_time) {
		this->player_controller->gain(delta_time);

		AnimationEntity2D::Gain(delta_time);
	}

}

