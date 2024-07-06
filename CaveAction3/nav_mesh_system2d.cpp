#include "nav_mesh_system2d.h"


void NavMeshSystem2D::add_rect(CAT_NMRect nmrect) {

	this->nmrect_lists.push_back(new CAT_NMRect());
	CAT_NMRect* rect_ptr = (this->nmrect_lists[this->nmrect_lists.size() - 1]);
	*rect_ptr = nmrect;

	/* ここから汎用性が無い */

	this->nmrect_map[nmrect.center[1]].push_back(rect_ptr);

	/* vectorとmap、それぞれに長方形メッシュのポインタが格納された */
}

int NavMeshSystem2D::height_to_key(int height) {

	for (auto it = nmrect_map.begin(); it != nmrect_map.end(); it++) {
		if (mins[it->first]-1 <= height && height <= maxs[it->first]+1) {
			//debug::debugLog("%d %d\n", mins[it->first], maxs[it->first]);
			return it->first;
		}
	}
}

NavMeshSystem2D::NavMeshSystem2D(XMLData* rect_data, std::vector<std::vector<unsigned short>>* bake_data, Eigen::Vector2i offset) {
	std::vector<CAT_NMRect> rects = XMLLoader::NMRectLoad(rect_data, offset);

	for (int i = 0; i < rects.size(); i++) {
		add_rect(rects[i]);
	}

	for (auto it = nmrect_map.begin(); it != nmrect_map.end(); it++) {
		maxs[it->first] = it->second[0]->bottom;
		mins[it->first] = it->second[0]->top;

		//debug::debugLog("key : %d , top : %d , bottom : %d\n", it->first, mins[it->first], maxs[it->first]);

	}


	

	for (int i = 0; i < bake_data->size(); i++) {
		std::vector<unsigned short> row_table = (*bake_data)[i];
		this->id_map[{row_table[0], row_table[1]}] = { row_table[2], row_table[3] };
	}

	//debug::debugLog("next : %d, length : %d\n", id_map[{0, 10}].first, id_map[{0,10}].second);

}

NavMeshSystem2D::~NavMeshSystem2D() {
	for (int i = 0; i < this->nmrect_lists.size(); i++) {
		delete this->nmrect_lists[i]; /* 長方形メッシュのメモリ解放 */
	}
}

CAT_NMRect* NavMeshSystem2D::get_rect(unsigned short id) {
	return this->nmrect_lists[id];
}

std::vector<CAT_NMRect*> NavMeshSystem2D::get_all_rect(){
	return this->nmrect_lists;
}

unsigned short NavMeshSystem2D::get_id(Eigen::Vector2i center_ptr) {
	std::vector<CAT_NMRect*>* vec_ptr = &(nmrect_map[height_to_key(center_ptr[1])]);

	for (int i = 0; i < vec_ptr->size(); i++) {
		CAT_NMRect* rect_ptr = (*vec_ptr)[i];

		if (rect_ptr->left - NAVMESH_OFFSET <= center_ptr[0] && center_ptr[0] <= rect_ptr->right + NAVMESH_OFFSET) {
			return rect_ptr->id;
		}
		debug::debugLog("Error : Failed to get NMRect ID!\n");
	}

	
	//debug::debugLog("%d %d\n", center_ptr[0],height_to_key(center_ptr[1]));
	//debug::debugLog("%d %d\n", mins[height_to_key(center_ptr[1])], maxs[height_to_key(center_ptr[1])]);
	return 255;


}

unsigned short  NavMeshSystem2D::get_next_id(unsigned short start_id, unsigned short end_id) {
	return this->id_map[{start_id, end_id}].first;
}

std::pair<unsigned short, unsigned short> NavMeshSystem2D::get_next_info(unsigned short start_id, unsigned short end_id) {
	return this->id_map[{start_id, end_id}];
}