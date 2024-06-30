#pragma once

#include <vector>
#include <list>

#include "image_root.h"

#define MAX_IMAGE_LAYER_SIZE (64)


class ImageProjecter {
private:
	std::map<std::string, std::list<component::CAT_ImageRoot*>> image_map;
	std::vector<std::vector<component::CAT_ImageRoot**>> image_table;
	component::CAT_ViewCamera* m_viewcamera;
public:

	ImageProjecter();
	~ImageProjecter();

	void set_camera(component::CAT_ViewCamera* viewcamera);

	void save_generate_object(component::CAT_ImageRoot* imageRoot, unsigned char layer, std::string object_name);
	void project();

	void cancel(std::string object_name);

};