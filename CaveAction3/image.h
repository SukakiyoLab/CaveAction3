#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "transform.h"

#include "raw_image.h"
#include "image_projecter.h"

namespace component
{

	class CAT_Image : public CAT_RawImage 
	{
	private:
		SDL_Surface *m_image; // 画像 //
		int m_w;			  // 幅 // 
		int m_h;			  // 高さ //
		SDL_Renderer *m_renderer;
		SDL_Texture *m_texture;
		SDL_Rect m_image_rect;
		SDL_FRect m_draw_rect;
		Uint32 m_format;
		
	public:
		CAT_Image(CAT_Transform *const transform, const char *path, SDL_Renderer *const renderer);
		void project() override;
	};
}
