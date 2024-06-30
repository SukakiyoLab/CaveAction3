#include "collider_manager.h"

#include <vector>

ColliderManager::ColliderManager() {
	this->collider_table = std::vector<std::vector<component::CAT_Collider2D*>>(MAX_COLLIDER_LAYER_SIZE);
}

ColliderManager::~ColliderManager() {

}

void ColliderManager::save_generate_object(component::CAT_Collider2D* collider, unsigned char layer, std::string object_name) {
	this->collider_table[layer].push_back(collider);
	this->collider_map[object_name].push_back({ layer, this->collider_table[layer].size() - 1 });
}

void ColliderManager::judge() {
	for (int i = 0; i < this->collider_table.size(); i++) {
		for (int j = 0; j < this->collider_table[i].size(); j++) {

			if (this->collider_table[i][j] == nullptr) {
				continue;
			}

			//this->collider_table[i][j]->update();
			for (int k = 0; k < this->collider_table[i].size(); k++) {
				if (k != j) {
					if (this->collider_table[i][k] == nullptr) {
						continue;
					}

					this->collider_table[i][j]->judge(this->collider_table[i][k]);
				}
			}
		}
	}
}

void ColliderManager::cancel(std::string object_name) {
	std::vector<std::pair<unsigned short, unsigned short>> index_pair_vec = this->collider_map[object_name];

	for (auto index_pair : index_pair_vec) {
		this->collider_table[index_pair.first][index_pair.second] = nullptr;
	}
}



