#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "transform.h"

#include <vector>

#include "image_root.h"
#include "animation_root.h"

namespace component
{

	class CAT_AnimationImage : public CAT_ImageRoot, public CAT_AnimationRoot
	{
	private:
		struct ImageState{
			std::vector<SDL_Surface*> m_images;
			std::vector<SDL_Texture*> m_textures;
			std::vector<SDL_Rect> m_image_rects;
			int m_w;
			int m_h;

			std::vector<double> duration_times;
		};



		//SDL_Surface *m_image; // 画像 //
		int m_w;			  // 幅 //
		int m_h;			  // 高さ //
		Eigen::Vector2i offset;
		SDL_Renderer *m_renderer;
		//SDL_Texture *m_texture; //
		//SDL_Rect m_image_rect; //
		SDL_FRect m_draw_rect;
		Uint32 m_format;
		
		double sum_time = 0;

		std::vector<ImageState> states;
		

	public:
		CAT_AnimationImage(CAT_Transform *const transform, const char* xml_file, SDL_Renderer *const renderer, Eigen::Vector2i offset);
		void gain(double delta_time);
		void project(CAT_ViewCamera* camera) override;
		int change_animation(unsigned short new_id) override;
		
	};
}
