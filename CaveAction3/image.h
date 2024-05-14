#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>


#include "transform.h"

#include "image_root.h"
#include "image_projecter.h"

namespace component
{

	class CAT_Image : public CAT_ImageRoot 
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

		float project_width;
		float project_height;
		Eigen::Vector2f project_offset;
		Uint8 alpha;

	public:
		struct ComponentInitializer : public CAT_ImageRoot::ComponentInitializer {
			const char* path;
			float width = 1.0f;
			float height = 1.0f; 
			Eigen::Vector2i offset = Vector2i{ 0, 0 };
			Uint8 image_alpha = 255;
			unsigned short image_layer = 0;

		};
		
	public:
		static  const char* get_component_name() { return "Image"; };
		//CAT_Image(CAT_Transform* const transform, const char* path, SDL_Renderer* const renderer);
		CAT_Image(CAT_Transform* const transform, ComponentInitializer* cInit , SDL_Renderer*const renderer_ptr);
		void project(CAT_ViewCamera* camera) override;

		
	};
}
