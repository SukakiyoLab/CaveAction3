#include "NavMeshEntity.h"

namespace object {
	NavMeshEntity::NavMeshEntity(ObjectInitializer objectInit)
		: GameObject::GameObject(static_cast<GameObject::ObjectInitializer>(objectInit)) {

#ifdef _DEBUG
		for (auto rect : objectInit.rect_list) {

			component::CAT_Image::ComponentInitializer debugInitImage;

			debugInitImage.path = "./resource/imgs/panel2.png";
			debugInitImage.width = (rect->right - rect->left) / 32;
			debugInitImage.height = (rect->bottom - rect->top) / 32;
			debugInitImage.offset = Eigen::Vector2i(rect->center[0], rect->center[1]);
			debugInitImage.image_alpha = 100;
			debugInitImage.image_layer = 7;

			image_list.push_back(new component::CAT_Image(this->transform,
				&debugInitImage,
				objectInit.renderer_ptr
			));
			objectInit.projecter->save(image_list[image_list.size() - 1], debugInitImage.image_layer);
		}
#endif
	}

	NavMeshEntity::~NavMeshEntity() {
		for (auto image : image_list) {
			delete image;
		}
	}
}