#include "image_projecter.h"

#include "image.h"
#include "animation_image.h"
#include "tilemap.h"


ImageProjecter::ImageProjecter() {
	
	this->image_table = std::vector<std::vector<component::CAT_ImageRoot**>>(MAX_IMAGE_LAYER_SIZE);
}

ImageProjecter::~ImageProjecter() {

}

void ImageProjecter::set_camera(component::CAT_ViewCamera* viewcamera) {
	this->m_viewcamera = viewcamera;
}

void ImageProjecter::save_generate_object(component::CAT_ImageRoot* rawImage, unsigned char layer, std::string object_name){
	
	this->image_map[object_name].push_back(rawImage);

	auto end_ptr = this->image_map[object_name].end();

	--end_ptr;

	this->image_table[layer].push_back(&(*end_ptr));
}

void ImageProjecter::project() {
	for (int i = 0; i < this->image_table.size(); i++) {



		std::sort(this->image_table[i].begin(), this->image_table[i].end(), component::CAT_ImageRoot::cmp);


		for (int j = 0; j < this->image_table[i].size(); j++) {

			if (*(this->image_table[i][j]) == nullptr) {
				continue;
			}

			(*(this->image_table[i][j]))->project(this->m_viewcamera);	
		}
	}
}


void ImageProjecter::cancel(std::string object_name) {
	for (auto image_ptr : image_map[object_name]) {
		image_ptr = nullptr;
	}
}


