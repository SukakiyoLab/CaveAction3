#pragma once

#include "collider2d.h"
#include <vector>

#define MAX_COLLIDER_LAYER_SIZE (64)


class ColliderManager {
private:
	std::map<std::string, std::vector<std::pair<unsigned short, unsigned short>>> collider_map;
	std::vector<std::vector<component::CAT_Collider2D*>> collider_table;

public:
	ColliderManager();
	~ColliderManager();

	void save_generate_object(component::CAT_Collider2D* collider, unsigned char layer, std::string object_name);
	void judge();

	void cancel(std::string object_name);
};


