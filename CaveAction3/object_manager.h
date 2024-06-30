#pragma once

#include <vector>
#include <map>

#include "GameObject.h"

#include "object_generator.h"

namespace game_system_1 {

	class ObjectManager {
	private:
		std::list<object::GameObject*> m_objects;
		std::map<std::string, object::GameObject**> m_objects_map;

		game_system_3::ObjectGenerator* m_generator_ptr;

		FunctionMap* m_func_map;
		CAT_Input* m_input_ptr = nullptr;

		ImageProjecter* m_projecter_ptr;
		ColliderManager* m_collider_manager_ptr;
		SDL_Renderer* m_renderer_ptr;
		NavMeshSystem2D* m_nav_mesh_system_ptr;

	public:
		struct ObjectManagerInitializer {
			CAT_Input* input_ptr;
			ImageProjecter* projecter_ptr;
			ColliderManager* coll_man_ptr;
			SDL_Renderer* renderer_ptr;
			NavMeshSystem2D* nav_mesh_system_ptr;
			FunctionMap* func_map_ptr;
		};

	public:
		ObjectManager(game_system_3::ObjectGenerator* generator_ptr, ObjectManagerInitializer omInit);
		~ObjectManager();
		void generate_objects();
		void delete_objects();
		void reset_generator();
		void gain(unsigned int delta_time);
		void update();
		std::pair<std::string, object::GameObject*> generate(XMLData* data_ptr);

	};


}