#pragma once
#include "transform.h"

#define DEFAULT_VIEWPORT_WIDTH (640)
#define DEFAULT_VIEWPORT_HEIGHT (480)

#include "component.h"

#include <string>

#include "xml_data.h"
#include "function_map.h"

namespace component {

	class CAT_ViewCamera : public CAT_Component{
	private:
		CAT_Transform* m_target;
		CAT_Transform* m_camera_transform;
		unsigned int m_width;
		unsigned int m_height;

		Eigen::Vector3d m_view_port_center;

	public:
		struct CAT_ViewPort {
			float top;
			float left;
			float right;
			float bottom;
		};

	public:
		struct ComponentInitializer : public CAT_Component::ComponentInitializer {
			Eigen::Vector3d view_port_center = Eigen::Vector3d(DEFAULT_VIEWPORT_WIDTH / 2, DEFAULT_VIEWPORT_HEIGHT / 2, 0);
			unsigned int width = DEFAULT_VIEWPORT_WIDTH * 1.5;
			unsigned int height = DEFAULT_VIEWPORT_HEIGHT * 1.5;
			std::string other_object_name;
			unsigned short other_transform_id = 0;
		};

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->view_port_center = Eigen::Vector3d(std::stod(xmldata_ptr->nexts["view_port_center"][0]->nexts["x"][0]->item),
				std::stod(xmldata_ptr->nexts["view_port_center"][0]->nexts["y"][0]->item),
				std::stod(xmldata_ptr->nexts["view_port_center"][0]->nexts["z"][0]->item));
			cInit_ptr->width = std::stoi(xmldata_ptr->nexts["width"][0]->item);
			cInit_ptr->height = std::stoi(xmldata_ptr->nexts["height"][0]->item);
			cInit_ptr->other_object_name = xmldata_ptr->nexts["other_object_name"][0]->item;
			cInit_ptr->other_transform_id = std::stoi(xmldata_ptr->nexts["other_transform_id"][0]->item);

			return cInit_ptr;
		}

	private:
		CAT_ViewPort m_view_port;
 
	public:
		CAT_ViewCamera(CAT_Transform* camera_transform, CAT_Transform* target_transform, ComponentInitializer* cInit);
		
		void update() override;

		void set_target(CAT_Transform* target);

		Eigen::Vector3d get_position();
		Eigen::Vector3d get_view_port_center();
		float get_view_port_param(unsigned char direction_num);
	};

}
