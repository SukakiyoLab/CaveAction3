#include "GameObject.h"


#include <typeinfo>
//#include
#include <string>
namespace object {



	template<class T> 
	std::string get_component_name<T>() {
		std::string text = std::string(typeid(T).name());
		
		return text.substr(text.find(":") + 2);
	}

	GameObject::GameObject(GameObject::ObjectInitializer* objectInit) {
		this->name = objectInit->name;
		this->transform = new component::CAT_Transform(&(objectInit->transformInit));

		bool error_id = false;

		debug::debugLog("name : %s\n", this->name.c_str());
		
		
		for (component::CAT_Component::ComponentInitializer* componentInit_ptr : objectInit->selfComponentInits) {
			component::CAT_Component* component_ptr;
			

			//printf("Hello World!\n");
				
			component::CAT_AnimationImage::ComponentInitializer* animationImage_ptr = dynamic_cast<component::CAT_AnimationImage::ComponentInitializer*>(componentInit_ptr);
			if (animationImage_ptr != nullptr) {
				//printf("Hello AnimationImage!\n");
				component_ptr = new component::CAT_AnimationImage(this->transform, animationImage_ptr, objectInit->renderer_ptr);
				self_components_map[get_component_name<component::CAT_AnimationImage>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				objectInit->projecter_ptr->save_generate_object(dynamic_cast<component::CAT_ImageRoot*>(component_ptr), animationImage_ptr->image_layer, this->name);

				continue;
			}

			component::CAT_AnimationCurve::ComponentInitializer* animationCurve_ptr = dynamic_cast<component::CAT_AnimationCurve::ComponentInitializer*>(componentInit_ptr);
			if (animationCurve_ptr != nullptr) {
				component_ptr = new component::CAT_AnimationCurve(animationCurve_ptr);
				self_components_map[get_component_name<component::CAT_AnimationCurve>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);

				continue;
			}


			component::CAT_Animator2D::ComponentInitializer* animator2DInit_ptr = dynamic_cast<component::CAT_Animator2D::ComponentInitializer*>(componentInit_ptr);
			if (animator2DInit_ptr != nullptr) {
				auto anim_root_compo_vector = &(self_components_map[animator2DInit_ptr->anim_type]);
				if (animator2DInit_ptr->anim_root_id >= anim_root_compo_vector->size()) {
					error_id = true;
				}

				component_ptr = new component::CAT_Animator2D(dynamic_cast<component::CAT_AnimationRoot*>((*anim_root_compo_vector)[animator2DInit_ptr->anim_root_id]), animator2DInit_ptr);
				self_components_map[get_component_name<component::CAT_Animator2D>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;
			}

			component::CAT_BoxCollider2D::ComponentInitializer* boxColliderInit_ptr = dynamic_cast<component::CAT_BoxCollider2D::ComponentInitializer*>(componentInit_ptr);
			if (boxColliderInit_ptr != nullptr) {
				auto rigidbody_compo_vector = &(self_components_map[get_component_name<component::CAT_Rigidbody>()]);
				if (boxColliderInit_ptr->rigidbody_id >= rigidbody_compo_vector->size()) {
					error_id = true;
				}

				component_ptr = new component::CAT_BoxCollider2D(this->transform,
					dynamic_cast<component::CAT_Rigidbody*>((*rigidbody_compo_vector)[boxColliderInit_ptr->rigidbody_id]),
					boxColliderInit_ptr);
				self_components_map[get_component_name<component::CAT_BoxCollider2D>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				objectInit->collider_manager_ptr->save_generate_object(dynamic_cast<component::CAT_Collider2D*>(component_ptr), boxColliderInit_ptr->layer, this->name);

#ifdef _DEBUG
				component::CAT_BoxCollider2D::Range range = (dynamic_cast<component::CAT_BoxCollider2D*>(component_ptr))->get_range();
				component::CAT_Image::ComponentInitializer debugImageInit;
				debugImageInit.path = "./resource/imgs/panel1.png";
				debugImageInit.width = range.width / 32;
				debugImageInit.height = range.height / 32;
				debugImageInit.offset = range.offset.cast<int>();
				debugImageInit.image_alpha = 100;
				debugImageInit.image_layer = 8;
				component::CAT_Image* debug_image = new component::CAT_Image(this->transform, &debugImageInit, objectInit->renderer_ptr);
				self_components_vector.push_back(debug_image);
				objectInit->projecter_ptr->save_generate_object(debug_image, debugImageInit.image_layer, this->name);
#endif

				continue;
			}

			component::CAT_Image::ComponentInitializer* image_ptr = dynamic_cast<component::CAT_Image::ComponentInitializer*>(componentInit_ptr);
			if (image_ptr != nullptr) {

				component_ptr = new component::CAT_Image(this->transform, image_ptr, objectInit->renderer_ptr);
				self_components_map[get_component_name<component::CAT_Image>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;
			}

			component::CAT_NavMeshAgent2D::ComponentInitializer* navMeshAgent2DInit_ptr = dynamic_cast<component::CAT_NavMeshAgent2D::ComponentInitializer*>(componentInit_ptr);
			if (navMeshAgent2DInit_ptr != nullptr) {
				component_ptr = new component::CAT_NavMeshAgent2D(this->transform, 
					dynamic_cast<component::CAT_Transform*>(objectInit->other_obj_components[navMeshAgent2DInit_ptr->target_object_name + "::" + get_component_name<component::CAT_Transform>()][navMeshAgent2DInit_ptr->target_transform_id]),
					objectInit->nav_mesh_system_ptr);
				self_components_map[get_component_name<component::CAT_NavMeshAgent2D>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;
			}

			component::CAT_Rigidbody::ComponentInitializer* rigidbodyInit_ptr = dynamic_cast<component::CAT_Rigidbody::ComponentInitializer*>(componentInit_ptr);
			if (rigidbodyInit_ptr != nullptr) {
				//printf("Hello Rigidbody!\n");
				component_ptr = new component::CAT_Rigidbody(this->transform, rigidbodyInit_ptr);
				self_components_map[get_component_name<component::CAT_Rigidbody>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;
			}

			component::CAT_TileCollider2D::ComponentInitializer* tileColliderInit_ptr = dynamic_cast<component::CAT_TileCollider2D::ComponentInitializer*>(componentInit_ptr);
			if (tileColliderInit_ptr != nullptr) {
				auto rigidbody_compo_vector = &(self_components_map[get_component_name<component::CAT_Rigidbody>()]);
				component::CAT_Rigidbody* rigidbody_ptr;
				if (tileColliderInit_ptr->rigidbody_id >= rigidbody_compo_vector->size()) {
					rigidbody_ptr = nullptr;
				}
				else {
					rigidbody_ptr = dynamic_cast<component::CAT_Rigidbody*>((*rigidbody_compo_vector)[tileColliderInit_ptr->rigidbody_id]);
				}

				component_ptr = new component::CAT_TileCollider2D(this->transform,
					rigidbody_ptr,
					tileColliderInit_ptr);
				self_components_map[get_component_name<component::CAT_TileCollider2D>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				
				objectInit->collider_manager_ptr->save_generate_object(dynamic_cast<component::CAT_Collider2D*>(component_ptr), tileColliderInit_ptr->layer, this->name);
				if (tileColliderInit_ptr->layer < DEFAULT_MAX_COLLISION_LAYER && tileColliderInit_ptr->layer >= 0) {
#ifdef _DEBUG

					int size_h = tileColliderInit_ptr->tilemap_collider_init->size();
					int size_w = 0;

					if (size_h > 0) {
						size_w = (*tileColliderInit_ptr->tilemap_collider_init)[0].size();
					}

					//std::vector<std::vector<unsigned short>> debug_collider_init;

					component::CAT_Tilemap::ComponentInitializer debugmapInit;

					std::vector<std::vector<unsigned short>> debugmap_collider_init;

					for (int i = 0; i < size_h; i++) {
						std::vector<unsigned short> raw_init;
						for (int j = 0; j < size_w; j++)
						{
							int number = (*tileColliderInit_ptr->tilemap_collider_init)[i][j];
							if (number == 0) {
								raw_init.push_back(99);
							}
							else if (number == 1) {
								raw_init.push_back(1);
							}
						}
						debugmap_collider_init.push_back(raw_init);
					}
					this->debug_value_vector.push_back(debugmap_collider_init);

					debugmapInit.tilemap_init = &(this->debug_value_vector[this->debug_value_vector.size()-1]);
					debugmapInit.tilemap_path = "./resource/imgs/tilemap_debug1.png";
					debugmapInit.image_layer = 8;
					debugmapInit.image_alpha = 100;


					component::CAT_Tilemap* debug_map = new component::CAT_Tilemap(this->transform,
						&debugmapInit,
						objectInit->renderer_ptr);
					self_components_vector.push_back(debug_map);
					objectInit->projecter_ptr->save_generate_object(debug_map, debugmapInit.image_layer, this->name);
#endif
				}

				continue;
			}

			component::CAT_Tilemap::ComponentInitializer* tilemapInit_ptr = dynamic_cast<component::CAT_Tilemap::ComponentInitializer*>(componentInit_ptr);
			if (tilemapInit_ptr != nullptr) {

				component_ptr = new component::CAT_Tilemap(this->transform, tilemapInit_ptr, objectInit->renderer_ptr);
				self_components_map[get_component_name<component::CAT_Tilemap>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				objectInit->projecter_ptr->save_generate_object(dynamic_cast<component::CAT_ImageRoot*>(component_ptr), tilemapInit_ptr->image_layer, this->name);
				continue;
			}

			component::CAT_ViewCamera::ComponentInitializer* viewCameraInit_ptr = dynamic_cast<component::CAT_ViewCamera::ComponentInitializer*>(componentInit_ptr);
			if (viewCameraInit_ptr != nullptr) {
				auto other_transform_vector = &(objectInit->other_obj_components[viewCameraInit_ptr->other_object_name + "::" + get_component_name<component::CAT_Transform>()]);
				if (viewCameraInit_ptr->other_transform_id >= other_transform_vector->size()) {
					error_id = true;
				}


				component_ptr = new component::CAT_ViewCamera(this->transform,
					dynamic_cast<component::CAT_Transform*>((*other_transform_vector)[viewCameraInit_ptr->other_transform_id]),
					viewCameraInit_ptr);
				self_components_map[get_component_name<component::CAT_ViewCamera>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				objectInit->projecter_ptr->set_camera(dynamic_cast<component::CAT_ViewCamera*>(component_ptr));
				continue;
			}

			component::CAT_VirtualController::ComponentInitializer* virtualControllerInit_ptr = dynamic_cast<component::CAT_VirtualController::ComponentInitializer*>(componentInit_ptr);
			if (virtualControllerInit_ptr != nullptr) {
				auto rigidbody_compo_vector = &(self_components_map[get_component_name<component::CAT_Rigidbody>()]);
				if (virtualControllerInit_ptr->rigidbody_id >= rigidbody_compo_vector->size()) {
					error_id = true;
				}

				component_ptr = new component::CAT_VirtualController(dynamic_cast<component::CAT_Rigidbody*>((*rigidbody_compo_vector)[virtualControllerInit_ptr->rigidbody_id]), virtualControllerInit_ptr);
				self_components_map[get_component_name<component::CAT_VirtualController>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;
			}

			/* オリジナル・コンポーネント */

			component::CAT_BallController::ComponentInitializer* ballControllerInit_ptr = dynamic_cast<component::CAT_BallController::ComponentInitializer*>(componentInit_ptr);
			if (ballControllerInit_ptr != nullptr) {
				auto anim_image_compo_vector = &(self_components_map[get_component_name<component::CAT_AnimationImage>()]);
				if (ballControllerInit_ptr->animation_image_id >= anim_image_compo_vector->size()) {
					error_id = true;
				}

				auto rigidbody_compo_vector = &(self_components_map[get_component_name<component::CAT_Rigidbody>()]);
				if (ballControllerInit_ptr->rigidbody_id >= rigidbody_compo_vector->size()) {
					error_id = true;
				}
				auto anim_curve_compo_vector = &(self_components_map[get_component_name<component::CAT_AnimationCurve>()]);
				if (ballControllerInit_ptr->animation_curve_id >= anim_curve_compo_vector->size()) {
					error_id = true;
				}

				component_ptr = new component::CAT_BallController(
					this->transform,
					dynamic_cast<component::CAT_AnimationImage*>((*anim_image_compo_vector)[ballControllerInit_ptr->animation_image_id]),
					dynamic_cast<component::CAT_Rigidbody*>((*rigidbody_compo_vector)[ballControllerInit_ptr->rigidbody_id]),
					dynamic_cast<component::CAT_AnimationCurve*>((*anim_curve_compo_vector)[ballControllerInit_ptr->animation_curve_id]),
					ballControllerInit_ptr
				);

				self_components_map[get_component_name<component::CAT_BallController>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);

				continue;
			}

			component::CAT_PlayerController::ComponentInitializer* playerControllerInit_ptr = dynamic_cast<component::CAT_PlayerController::ComponentInitializer*>(componentInit_ptr);
			if (playerControllerInit_ptr != nullptr) {

				auto rigidbody_compo_vector = &(self_components_map[get_component_name<component::CAT_Rigidbody>()]);
				if (playerControllerInit_ptr->rigidbody_id >= rigidbody_compo_vector->size()) {
					error_id = true;
				}
				auto vc_compo_vector = &(self_components_map[get_component_name<component::CAT_VirtualController>()]);
				if (playerControllerInit_ptr->virtual_controller_id >= vc_compo_vector->size()) {
					error_id = true;
				}
				auto animator_compo_vector = &(self_components_map[get_component_name<component::CAT_Animator2D>()]);
				if (playerControllerInit_ptr->animator2D_id >= animator_compo_vector->size()) {
					error_id = true;
				}

				component_ptr = new component::CAT_PlayerController(
					this->transform,
					dynamic_cast<component::CAT_Rigidbody*>((*rigidbody_compo_vector)[playerControllerInit_ptr->rigidbody_id]),
					dynamic_cast<component::CAT_VirtualController*>((*vc_compo_vector)[playerControllerInit_ptr->virtual_controller_id]),
					dynamic_cast<component::CAT_Animator2D*>((*animator_compo_vector)[playerControllerInit_ptr->animator2D_id]),
					objectInit->generator_ptr,
					objectInit->function_map_ptr,
					playerControllerInit_ptr
				);

				self_components_map[get_component_name<component::CAT_PlayerController>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;

			}

			component::CAT_SlimeController::ComponentInitializer* slimeControllerInit_ptr = dynamic_cast<component::CAT_SlimeController::ComponentInitializer*>(componentInit_ptr);
			if (slimeControllerInit_ptr != nullptr) {

				auto rigidbody_compo_vector = &(self_components_map[get_component_name<component::CAT_Rigidbody>()]);
				if (slimeControllerInit_ptr->rigidbody_id >= rigidbody_compo_vector->size()) {
					error_id = true;
				}
				auto vc_compo_vector = &(self_components_map[get_component_name<component::CAT_VirtualController>()]);
				if (slimeControllerInit_ptr->virtual_controller_id >= vc_compo_vector->size()) {
					error_id = true;
				}
				auto animator_compo_vector = &(self_components_map[get_component_name<component::CAT_Animator2D>()]);
				if (slimeControllerInit_ptr->animator2D_id >= animator_compo_vector->size()) {
					error_id = true;
				}
				auto nav_mesh_agent_vector = &(self_components_map[get_component_name<component::CAT_NavMeshAgent2D>()]);
				if (slimeControllerInit_ptr->nav_mesh_agent_id >= nav_mesh_agent_vector->size()) {
					error_id = true;
				}

				component_ptr = new component::CAT_SlimeController(
					dynamic_cast<component::CAT_Rigidbody*>((*rigidbody_compo_vector)[slimeControllerInit_ptr->rigidbody_id]),
					dynamic_cast<component::CAT_VirtualController*>((*vc_compo_vector)[slimeControllerInit_ptr->virtual_controller_id]),
					dynamic_cast<component::CAT_Animator2D*>((*animator_compo_vector)[slimeControllerInit_ptr->animator2D_id]),
					dynamic_cast<component::CAT_NavMeshAgent2D*>((*nav_mesh_agent_vector)[slimeControllerInit_ptr->nav_mesh_agent_id]),
					slimeControllerInit_ptr
				);

				self_components_map[get_component_name<component::CAT_SlimeController>()].push_back(component_ptr);
				self_components_vector.push_back(component_ptr);
				continue;
			}


				
		}
		

	}

	GameObject::~GameObject() {
		//delete this->transform;
		pointer_wrapper::Delete<component::CAT_Transform>(this->transform);

		for (component::CAT_Component* component_ptr : this->self_components_vector) {
			if (component_ptr == nullptr) {
				delete component_ptr;
			}
		}
	}

	void GameObject::Update() {
		for (auto vec_item_ptr : self_components_vector) {
			vec_item_ptr->update();
		}
	}

	void GameObject::Gain(int delta_time) {
		for (auto vec_item_ptr : self_components_vector) {
			vec_item_ptr->gain(delta_time);
		}
	}

}