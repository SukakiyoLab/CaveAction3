#include "object_manager.h"


namespace game_system_1 {

	ObjectManager::ObjectManager(game_system_3::ObjectGenerator* generator_ptr, ObjectManagerInitializer omInit) {
		m_generator_ptr = generator_ptr;

		m_input_ptr = omInit.input_ptr;

		m_projecter_ptr = omInit.projecter_ptr;
		m_collider_manager_ptr = omInit.coll_man_ptr;
		m_renderer_ptr = omInit.renderer_ptr;
		m_nav_mesh_system_ptr = omInit.nav_mesh_system_ptr;
		m_func_map = omInit.func_map_ptr;

	}

	ObjectManager::~ObjectManager() {
		for (auto object_ptr : this->m_objects) {
			if (object_ptr != nullptr) {
				delete object_ptr;
			}
		}
	}

	void ObjectManager::generate_objects() {

		for (XMLData* data_ptr : *(m_generator_ptr->get_generate_object_data())) {
			auto object_set = this->generate(data_ptr);

			this->m_objects.push_back(object_set.second);
			auto end_ptr = this->m_objects.end();
			--end_ptr;
			this->m_objects_map[object_set.first].push_back(&(*end_ptr));
		}
		
	}


	void ObjectManager::delete_objects() {
		for (std::string object_name : *(m_generator_ptr->get_delete_object_data())) {
			for (object::GameObject** object_ptr : this->m_objects_map[object_name]) {
				delete *object_ptr;
				*object_ptr = nullptr;
			}
			this->m_collider_manager_ptr->cancel(object_name);
			this->m_projecter_ptr->cancel(object_name);
		}
	}

	void ObjectManager::reset_generator() {
		m_generator_ptr->reset_generator();
	}


	void ObjectManager::gain(unsigned int delta_time) {
		for (object::GameObject* object_ptr : this->m_objects) {
			if (object_ptr != nullptr) {
				object_ptr->Gain(delta_time);
			}
		}
	}

	void ObjectManager::update() {
		for (object::GameObject* object_ptr : this->m_objects) {
			if (object_ptr != nullptr) {
				object_ptr->Update();
			}
		}
	}

	std::pair<std::string, object::GameObject*> ObjectManager::generate(XMLData* data_ptr) {
		object::GameObject::ObjectInitializer* gameObjectInit;
		gameObjectInit = object::GameObject::create_initializer(data_ptr, this->m_func_map,this->m_input_ptr);

		gameObjectInit->projecter_ptr = m_projecter_ptr;
		gameObjectInit->collider_manager_ptr = m_collider_manager_ptr;
		gameObjectInit->renderer_ptr = m_renderer_ptr;
		gameObjectInit->nav_mesh_system_ptr = m_nav_mesh_system_ptr;
		gameObjectInit->generator_ptr = m_generator_ptr;
		gameObjectInit->function_map_ptr = m_func_map;


		for (XMLData* other_component_data_ptr : data_ptr->nexts["other_object_components"][0]->nexts["object"]) {
			std::string object_name = other_component_data_ptr->item;
			std::string component_name = other_component_data_ptr->nexts["component"][0]->item;
			

			if (component_name == "CAT_Transform") {
				gameObjectInit->other_obj_components[object_name + "::" + component_name].push_back((*this->m_objects_map[object_name][0])->GetTransform());
			}
			else {
				unsigned short component_id = std::stoi(other_component_data_ptr->nexts["component_id"][0]->item);
				gameObjectInit->other_obj_components[object_name + "::" + component_name].push_back((*this->m_objects_map[object_name][0])->GetComponent(component_name, component_id));
			}
		}


		object::GameObject* new_object = new object::GameObject(gameObjectInit);
		std::string name = gameObjectInit->name;

		delete gameObjectInit;

		return std::pair<std::string, object::GameObject*>(name, new_object);
	}


}
