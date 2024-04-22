#pragma once

#include <eigen3/Eigen/Dense>

#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#include "transform.h"

namespace object {


	enum Type {
		Normal,
		Player,
		Enemy
	};

	class GameObject {
	protected:
		component::CAT_Transform* transform;
		Type type = Type::Normal;

	public:

		struct ObjectInitializer {
			Vector3d position;
			Vector3d rotation;
			Vector3d scale;

		};

		GameObject(GameObject::ObjectInitializer objectInit);
		virtual ~GameObject();


		virtual void Update();
		virtual void Gain(double delta_time);


		component::CAT_Transform* GetTransform() { return transform; }
		Vector3d GetPosition() { return transform->get_position(); }
		Vector3d GetRotation() { return transform->get_rotation(); }

		void SetPosition(Vector3d new_position) { transform->set_position(new_position); }

	};
}