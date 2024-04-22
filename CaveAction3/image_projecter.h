#pragma once

#include <vector>

#include "image_root.h"

#define MAX_IMAGE_LAYER_SIZE (64)


class ImageProjecter {
private:
	std::vector<std::vector<component::CAT_ImageRoot*>> image_table;
	component::CAT_ViewCamera* m_viewcamera;
public:

	ImageProjecter();
	~ImageProjecter();

	void set_camera(component::CAT_ViewCamera* viewcamera);

	void save(component::CAT_ImageRoot* rawImage, unsigned char layer);
	void project();

};