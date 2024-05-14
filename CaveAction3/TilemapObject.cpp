#include "TilemapObject.h"

namespace object {

	TilemapObject::TilemapObject(TilemapObject::ObjectInitializer objectInit) 
	: GameObject::GameObject(static_cast<GameObject::ObjectInitializer>(objectInit)) {

		this->tilemap = new component::CAT_Tilemap(this->transform,
													&(objectInit.tilemapInit),
													objectInit.renderer_ptr);

		objectInit.projecter_ptr->save(this->tilemap, objectInit.tilemapInit.image_layer);

		if (objectInit.tileColliderInit.layer < MAX_COLLIDER_LAYER_SIZE - 1) {

			this->tile_collider = new component::CAT_TileCollider2D(
				this->transform,
				nullptr,
				&(objectInit.tileColliderInit)
			);

			objectInit.collider_manager_ptr->save(this->tile_collider, objectInit.tileColliderInit.layer);

#ifdef _DEBUG

			int size_w = objectInit.tileColliderInit.tilemap_collider_init.size();
			int size_h = 0;

			if (size_w > 0) {
				size_h = objectInit.tileColliderInit.tilemap_collider_init[0].size();
			}

			//std::vector<std::vector<unsigned short>> debug_collider_init;

			component::CAT_Tilemap::ComponentInitializer debugmapInit;

			for (int i = 0; i < size_w; i++) {
				std::vector<unsigned short> raw_init;
				for (int j = 0; j < size_h; j++)
				{
					int number = objectInit.tileColliderInit.tilemap_collider_init[i][j];
					if (number == 0) {
						raw_init.push_back(99);
					}
					else if (number == 1) {
						raw_init.push_back(1);
					}
				}
				debugmapInit.tilemap_init.push_back(raw_init);
			}

			debugmapInit.tilemap_path = "./resource/imgs/tilemap_debug1.png";
			debugmapInit.image_layer = 8;
			debugmapInit.image_alpha = 100;


			this->debug_map = new component::CAT_Tilemap(this->transform,
				&debugmapInit,
				objectInit.renderer_ptr);

			objectInit.projecter_ptr->save(this->debug_map, debugmapInit.image_layer);

#endif
		}

	}

	TilemapObject::~TilemapObject() {
		delete this->tilemap;

		if (this->tile_collider!=nullptr) {
			delete this->tile_collider;
#ifdef _DEBUG
			delete this->debug_map;
#endif
		}
	}

}