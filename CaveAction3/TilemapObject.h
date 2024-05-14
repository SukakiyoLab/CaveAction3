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
			component::CAT_Tilemap::ComponentInitializer tilemapInit;
			//ImageProjecter* projecter;


			component::CAT_TileCollider2D::ComponentInitializer tileColliderInit;

			//unsigned short collider_layer;
			//double collider_width;
			//std::vector<std::vector<unsigned short>> tilemap_collider_init;
			//float collision_magnitude;
			//int collision;
			//ColliderManager* collider_manager;

#ifdef _DEBUG
			component::CAT_Tilemap::ComponentInitializer debugmapInit;
#endif


		};

		TilemapObject(ObjectInitializer objectInit);
		~TilemapObject();

	};


}