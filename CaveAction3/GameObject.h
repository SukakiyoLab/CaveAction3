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

#include "animation_image.h"
#include "animation_root.h"
#include "animator2d.h"
#include "boxcollider2d.h"
#include "collider2d.h"
#include "image.h"
#include "image_root.h"
#include "nav_mesh_agent2d.h"
#include "rigidbody.h"
#include "tilecollider2d.h"
#include "tilemap.h"
#include "view_camera.h"
#include "virtual_controller.h"

#include "character_controller.h"
#include "player_controller.h"
#include "slime_controller.h"

#include "pointer_wrapper.h"

#include "xml_data.h"


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

#ifdef _DEBUG
		std::vector<std::vector<std::vector<unsigned short>>> debug_value_vector;
#endif


	public:

		struct ObjectInitializer {
			std::string name;
			Type type;
			ImageProjecter* projecter_ptr;
			ColliderManager* collider_manager_ptr;
			SDL_Renderer* renderer_ptr;
			NavMeshSystem2D* nav_mesh_system_ptr;

			std::map<std::string, std::vector<component::CAT_Component*>> other_obj_components;

			component::CAT_Transform::ComponentInitializer transformInit;

			std::vector<component::CAT_Component::ComponentInitializer*> selfComponentInits;

		public:
			inline virtual ~ObjectInitializer() {
				/*for (auto com_init_ptr : selfComponentInits) {
					delete com_init_ptr;
				}*/
			}
		};

		static ObjectInitializer* create_initializer(XMLData* data_ptr, FunctionMap* funcMap_ptr, CAT_Input* input_ptr) {
			ObjectInitializer* objInit_ptr = new ObjectInitializer();
			objInit_ptr->name = data_ptr->nexts["name"][0]->item;
			if (data_ptr->nexts["type"][0]->item == "Normal") {
				objInit_ptr->type = Type::Normal;
			}
			else if (data_ptr->nexts["type"][0]->item == "Enemy") {
				objInit_ptr->type = Type::Enemy;
			}
			else if (data_ptr->nexts["type"][0]->item == "Player") {
				objInit_ptr->type = Type::Player;
			}

			XMLData* child_trans_ptr = data_ptr->nexts["transform"][0];
			if (child_trans_ptr != nullptr) {
				component::CAT_Transform::ComponentInitializer* transform_init_ptr = component::CAT_Transform::create_initializer(child_trans_ptr, funcMap_ptr);
				objInit_ptr->transformInit = *(transform_init_ptr);
				delete transform_init_ptr;
			}

			for (XMLData* root_component_ptr : data_ptr->nexts["component"]) {

				for (XMLData* child_ptr : root_component_ptr->nexts["animation_image"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_AnimationImage::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["animator2d"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_Animator2D::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["box_collider2d"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_BoxCollider2D::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["image"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_Image::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}
				for (XMLData* child_ptr : root_component_ptr->nexts["nav_mesh_agent2d"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_NavMeshAgent2D::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}
				for (XMLData* child_ptr : root_component_ptr->nexts["rigidbody"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_Rigidbody::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["tile_collider2d"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_TileCollider2D::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["tilemap"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_Tilemap::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["view_camera"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_ViewCamera::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["virtual_controller"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_VirtualController::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}



				for (XMLData* child_ptr : root_component_ptr->nexts["player_controller"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_PlayerController::create_initializer(child_ptr, funcMap_ptr, input_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

				for (XMLData* child_ptr : root_component_ptr->nexts["slime_controller"]) {
					component::CAT_Component::ComponentInitializer* com_ptr = component::CAT_SlimeController::create_initializer(child_ptr, funcMap_ptr);
					objInit_ptr->selfComponentInits.push_back(com_ptr);
				}

			}

			return objInit_ptr;

		}

		

		GameObject(GameObject::ObjectInitializer* objectInit);
		virtual ~GameObject();


		virtual void Update();
		virtual void Gain(int delta_time);


		inline component::CAT_Transform* GetTransform() { return transform; }
		inline component::CAT_Component* GetComponent(std::string component_name, unsigned short component_id) 
		{
			if (self_components_map[component_name].size() < component_id) {
				return nullptr;
			}

			return self_components_map[component_name][component_id]; 
		}

		Vector3d GetPosition() { return transform->get_position(); }
		Vector3d GetRotation() { return transform->get_rotation(); }

		void SetPosition(Vector3d new_position) { transform->set_position(new_position); }

	};
}