#pragma once

#include "shared_struct.h"

#include <map>
#include <vector>

#include <string>

#include "debug_log.h"

#include "xml_loader.h"

class NavMeshSystem2D {
private:
	std::map<int, std::vector<CAT_NMRect*>> nmrect_map;
	std::vector<CAT_NMRect*> nmrect_lists;

	std::map<int, int> maxs;
	std::map<int, int> mins;

	std::map<std::pair<unsigned short, unsigned short>, std::pair<unsigned short, unsigned short>> id_map;



private:
	void add_rect(CAT_NMRect nmrect);

	int height_to_key(int height);


public:

	NavMeshSystem2D(const char* rect_data, std::vector<std::vector<unsigned short>> bake_data, Eigen::Vector2i offset);
	~NavMeshSystem2D();

	CAT_NMRect* get_rect(unsigned short id);
	std::vector<CAT_NMRect*> get_all_rect();
	unsigned short get_id(Eigen::Vector2i position);

	unsigned short get_next_id(unsigned short start_id, unsigned short end_id);
	std::pair<unsigned short, unsigned short> get_next_info(unsigned short start_id, unsigned short end_id);

};