#include "image.h"

#include "image_storage.h"

#include "debug_log.h"

namespace component
{

	CAT_Image::CAT_Image(CAT_Transform *const transform, const char *path, SDL_Renderer *const renderer)
	:CAT_RawImage(transform){
		debug::debugLog("Create Image!\n");
		
		CAT_ImageStorage *storage = CAT_ImageStorage::getInstance();
		storage->save_image(path);
		this->m_image = storage->get_image(path);
		this->m_renderer = renderer;

		if (!this->m_image)
		{
			debug::debugLog("IMG_Load: %s\n", IMG_GetError());
		}

		SDL_SetColorKey(this->m_image,
						SDL_TRUE,
						SDL_MapRGB(this->m_image->format, 255, 0, 255));

		this->m_texture = SDL_CreateTextureFromSurface(m_renderer, m_image);

		
		SDL_QueryTexture(this->m_texture, &(this->m_format), NULL, &(this->m_w), &(this->m_h));
		this->m_image_rect = SDL_Rect{0, 0, m_w, m_h};
	}

	void CAT_Image::project()
	{
		float draw_w = this->m_w * this->m_transform->get_scale()[0];
		float draw_h = this->m_h * this->m_transform->get_scale()[1];

		// float draw_h = 32;
		// float draw_w = 32;

		Vector3d pos = this->m_transform->get_position();

		this->m_draw_rect = SDL_FRect{(float)pos[0] - draw_w / 2, (float)pos[1] - draw_h / 2, draw_w, draw_h};

		SDL_SetTextureAlphaMod(this->m_texture, 255);


		SDL_RenderCopyExF(this->m_renderer,
						  this->m_texture,
						  &(this->m_image_rect),
						  &(this->m_draw_rect),
						  0,
						  NULL,
						  SDL_FLIP_NONE);
	}
}