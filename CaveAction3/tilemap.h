#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "transform.h"

#include "raw_image.h"
#include "image_projecter.h"

#define TILE_SIZE (32)


namespace component
{
    class CAT_Tilemap : public CAT_RawImage
    {
    private:
		SDL_Surface *m_image; // 画像 //
		int m_w;			  // 幅 //
		int m_h;			  // 高さ //
		SDL_Renderer *m_renderer;
		SDL_Texture *m_texture;
		std::vector<SDL_Rect> m_image_rect;
		std::vector<SDL_Rect> m_draw_rect;
		Uint32 m_format;

        std::vector<std::vector<unsigned short>> tile_init;

		

	public:
		CAT_Tilemap(CAT_Transform *const transform, const char *path, std::vector<std::vector<unsigned short>> tilemap_init, SDL_Renderer *const renderer);
		void project() override;
    };
}