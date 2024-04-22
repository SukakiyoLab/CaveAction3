#pragma once

#include "shared_struct.h"

#include "GameObject.h"

#include "image.h"
#include <SDL.h>
#include <SDL_image.h>

namespace object {
	
	class NavMeshEntity : GameObject {
	private:
		std::vector<component::CAT_Image*> image_list;

	public:
		struct ObjectInitializer : GameObject::ObjectInitializer {
			std::vector<CAT_NMRect*> rect_list;
			SDL_Renderer* renderer;
			ImageProjecter* projecter;
		};

	public:
		NavMeshEntity(ObjectInitializer objectInit);
		~NavMeshEntity();
		

	};



}
