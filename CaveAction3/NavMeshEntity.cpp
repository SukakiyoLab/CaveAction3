#include "NavMeshEntity.h"

namespace object {
	NavMeshEntity::NavMeshEntity(ObjectInitializer objectInit)
		: GameObject::GameObject({
			objectInit.position,
			objectInit.rotation,
			objectInit.scale
			}) {

#ifdef _DEBUG
		for (auto rect : objectInit.rect_list) {
			image_list.push_back(new component::CAT_Image(this->transform,
				"./resource/imgs/panel2.png",
				objectInit.renderer,
				(rect->right - rect->left) / 32,
				(rect->bottom - rect->top) / 32,
				Eigen::Vector2f(rect->center[0], rect->center[1]),
				100
			));
			objectInit.projecter->save(image_list[image_list.size() - 1], 7);
		}
#endif
	}

	NavMeshEntity::~NavMeshEntity() {
		for (auto image : image_list) {
			delete image;
		}
	}
}