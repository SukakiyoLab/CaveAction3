#pragma once

#include "GameObject.h"
#include "tilemap.h"
#include "tilecollider2d.h"

#include "image_projecter.h"
#include "collider_manager.h"

namespace object {

	class TilemapObject : GameObject {
	private:
		component::CAT_Tilemap* tilemap;
		component::CAT_TileCollider2D* tile_collider;

#ifdef _DEBUG
		component::CAT_Tilemap* debug_map;
#endif


	public:
		struct ObjectInitializer : GameObject::ObjectInitializer {
			const char* tilemap_file_path;
			std::vector<std::vector<unsigned short>> tilemap_init;
			SDL_Renderer* renderer;
			unsigned short image_layer;
			Uint8 image_alpha = 255;
			ImageProjecter* projecter;


			unsigned short collider_layer;
			double collider_width;
			std::vector<std::vector<unsigned short>> tilemap_collider_init;
			float collision_magnitude;
			int collision;
			ColliderManager* collider_manager;

		};

		TilemapObject(ObjectInitializer objectInit);
		~TilemapObject();

	};


}