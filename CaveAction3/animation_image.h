#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

#include "transform.h"

#include <vector>

#include "image_root.h"
#include "animation_root.h"

#include "xml_data.h"

#include "function_map.h"

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
		//int m_w;			  // 幅 //
		//int m_h;			  // 高さ //
		Eigen::Vector2i offset;
		SDL_Renderer *m_renderer;
		//SDL_Texture *m_texture; //
		//SDL_Rect m_image_rect; //
		SDL_FRect m_draw_rect;
		Uint32 m_format;
		
		double sum_time = 0;

		std::vector<ImageState> states;

	public:
		struct ComponentInitializer : public CAT_ImageRoot::ComponentInitializer {
			XMLData* data_name;
			Eigen::Vector2i offset = Vector2i{ 0,0 };
			Uint8 image_alpha = 255;
			unsigned short image_layer = 0;
		};

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;

			cInit_ptr->data_name = funcMap_ptr->use_char_func(xmldata_ptr->nexts["data_name"][0]->item);
			cInit_ptr->offset = Vector2i{ std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["x"][0]->item), std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["y"][0]->item) };
			cInit_ptr->image_alpha = std::stoi(xmldata_ptr->nexts["image_alpha"][0]->item);
			cInit_ptr->image_layer = std::stoi(xmldata_ptr->nexts["image_layer"][0]->item);

			return cInit_ptr;
		}
		

	public:
		CAT_AnimationImage(CAT_Transform *const transform, ComponentInitializer* cInit, SDL_Renderer* renderer_ptr);
		void gain(int delta_time) override;
		void project(CAT_ViewCamera* camera) override;
		int change_animation(unsigned short new_id) override;
		
	};
}
