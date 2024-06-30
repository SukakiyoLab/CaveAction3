#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "transform.h"

#include "image_root.h"
#include "image_projecter.h"

#include "xml_data.h"
#include "function_map.h"

#define TILE_SIZE (32)


namespace component
{
    class CAT_Tilemap : public CAT_ImageRoot
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
		Uint8 m_alpha;

		Eigen::Vector2i m_offset;

        std::vector<std::vector<unsigned short>>* tile_init;

	public:
		struct ComponentInitializer : public CAT_ImageRoot::ComponentInitializer {
			const char* tilemap_path;
			std::vector<std::vector<unsigned short>>* tilemap_init;
			unsigned short image_layer;
			Uint8 image_alpha;
			Eigen::Vector2i offset = Eigen::Vector2i::Zero();
			//SDL_Renderer* renderer;
		};
		
		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->tilemap_path = xmldata_ptr->nexts["tilemap_path"][0]->item.c_str();
			cInit_ptr->tilemap_init = funcMap_ptr->use_value_func(xmldata_ptr->nexts["tilemap_init"][0]->item);
			cInit_ptr->image_layer = std::stoi(xmldata_ptr->nexts["image_layer"][0]->item);
			cInit_ptr->image_alpha = std::stoi(xmldata_ptr->nexts["image_alpha"][0]->item);
			cInit_ptr->offset = Eigen::Vector2i(std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["x"][0]->item), std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["y"][0]->item));

			return cInit_ptr;
		}


	public:
		CAT_Tilemap(CAT_Transform *const transform, ComponentInitializer* cInit, SDL_Renderer* renderer_ptr);
		void project(CAT_ViewCamera* camera) override;
    };
}