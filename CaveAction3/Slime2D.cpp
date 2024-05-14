#include "Slime2D.h"


namespace object {
	Slime2D::Slime2D(Slime2D::ObjectInitializer objectInit)
		: AnimationEntity2D::AnimationEntity2D(static_cast<AnimationEntity2D::ObjectInitializer>(objectInit)) {

		this->nm_agent_ptr = new component::CAT_NavMeshAgent2D(this->transform, 
			static_cast<component::CAT_Transform*>(objectInit.other_obj_components[objectInit.navMeshAgent2DInit.target_transform_name][objectInit.navMeshAgent2DInit.target_transform_id]), 
			objectInit.nav_mesh_system_ptr);

		this->slime_controller = new component::CAT_SlimeController(this->rigidbody, this->virtual_contoroller, this->animator_2d, this->nm_agent_ptr, &(objectInit.slimeInit));
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

	void Slime2D::Gain(int delta_time) {
		this->slime_controller->gain(delta_time);

		AnimationEntity2D::Gain(delta_time);
	}

}

