#pragma once

#include <eigen3/Eigen/Dense>

#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#include "transform.h"

#include "image_projecter.h"
#include "collider_manager.h"

#include <SDL.h>

#include "nav_mesh_system2d.h"

#include <string>
#include <map>

namespace object {

	template<class T>
	std::string get_component_name();

	enum Type {
		Normal,
		Player,
		Enemy
	};

	class GameObject {
	protected:
		std::string name;
		component::CAT_Transform* transform;
		Type type = Type::Normal;
		std::map<std::string, std::vector<component::CAT_Component*>> self_components_map;
		std::vector<component::CAT_Component*> self_components_vector;

	public:

		struct ObjectInitializer {
			ImageProjecter* projecter_ptr;
			ColliderManager* collider_manager_ptr;
			SDL_Renderer* renderer_ptr;
			NavMeshSystem2D* nav_mesh_system_ptr;

			std::map<std::string, std::vector<component::CAT_Component*>> other_obj_components;

			component::CAT_Transform::ComponentInitializer transformInit;

			std::vector<component::CAT_Component::ComponentInitializer*> selfComponentInits;
		};

		GameObject(GameObject::ObjectInitializer objectInit);
		virtual ~GameObject();


		virtual void Update();
		virtual void Gain(int delta_time);


		component::CAT_Transform* GetTransform() { return transform; }
		Vector3d GetPosition() { return transform->get_position(); }
		Vector3d GetRotation() { return transform->get_rotation(); }

		void SetPosition(Vector3d new_position) { transform->set_position(new_position); }

	};
}