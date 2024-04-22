#include "TilemapObject.h"

namespace object {

	TilemapObject::TilemapObject(TilemapObject::ObjectInitializer objectInit) 
	: GameObject::GameObject({
		objectInit.position,
		objectInit.rotation,
		objectInit.scale
		}) {

		this->tilemap = new component::CAT_Tilemap(this->transform,
													objectInit.tilemap_file_path,
													objectInit.tilemap_init,
													objectInit.image_alpha,
													objectInit.renderer);

		objectInit.projecter->save(this->tilemap, objectInit.image_layer);

		if (objectInit.collider_layer < MAX_COLLIDER_LAYER_SIZE - 1) {

			this->tile_collider = new component::CAT_TileCollider2D(
				this->transform,
				nullptr,
				objectInit.collider_layer,
				objectInit.collider_width,
				objectInit.tilemap_collider_init,
				objectInit.collision_magnitude,
				objectInit.collision
			);

			objectInit.collider_manager->save(this->tile_collider, objectInit.collider_layer);

#ifdef _DEBUG

			int size_w = objectInit.tilemap_collider_init.size();
			int size_h = 0;

			if (size_w > 0) {
				size_h = objectInit.tilemap_collider_init[0].size();
			}

			std::vector<std::vector<unsigned short>> debug_collider_init;

			for (int i = 0; i < size_w; i++) {
				std::vector<unsigned short> raw_init;
				for (int j = 0; j < size_h; j++)
				{
					int number = objectInit.tilemap_collider_init[i][j];
					if (number == 0) {
						raw_init.push_back(99);
					}
					else if (number == 1) {
						raw_init.push_back(1);
					}
				}
				debug_collider_init.push_back(raw_init);
			}

			this->debug_map = new component::CAT_Tilemap(this->transform,
				"./resource/imgs/tilemap_debug1.png",
				debug_collider_init,
				100,
				objectInit.renderer);

			objectInit.projecter->save(this->debug_map, 8);

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