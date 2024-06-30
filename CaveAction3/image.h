#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>


#include "transform.h"

#include "image_root.h"
#include "image_projecter.h"

#include "xml_data.h"
#include "function_map.h"

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

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->path = xmldata_ptr->nexts["path"][0]->item.c_str();
			cInit_ptr->width = std::stof(xmldata_ptr->nexts["width"][0]->item);
			cInit_ptr->height = std::stof(xmldata_ptr->nexts["height"][0]->item);
			cInit_ptr->offset = Vector2i{ std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["x"][0]->item), std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["y"][0]->item) };
			cInit_ptr->image_alpha = std::stoi(xmldata_ptr->nexts["image_alpha"][0]->item);
			cInit_ptr->image_layer = std::stoi(xmldata_ptr->nexts["image_layer"][0]->item);

			return cInit_ptr;
		}
		
	public:
		static  const char* get_component_name() { return "Image"; };
		//CAT_Image(CAT_Transform* const transform, const char* path, SDL_Renderer* const renderer);
		CAT_Image(CAT_Transform* const transform, ComponentInitializer* cInit , SDL_Renderer*const renderer_ptr);
		void project(CAT_ViewCamera* camera) override;

		
	};
}
