#pragma once


#include <string>
#include <vector>

#include "xml_data.h"

namespace game_system_3 {

	class ObjectGenerator {
	private:
		std::vector<XMLData*>* generate_object_data;

		std::vector<std::string>* delete_object_data;

	public:
		inline ObjectGenerator(){
			generate_object_data = new std::vector<XMLData*>();
			delete_object_data = new std::vector<std::string>();
		}

		inline ~ObjectGenerator() {
			delete generate_object_data;
		}

		inline void save_generate_object(XMLData* object_data) {
			generate_object_data->push_back(object_data);
		}

		inline void save_delete_object(std::string object_name) {
			delete_object_data->push_back(object_name);
		}

		inline std::vector<XMLData*>* get_generate_object_data() { 
			return this->generate_object_data; 
		}

		inline std::vector<std::string>* get_delete_object_data() {
			return this->delete_object_data;
		}

		inline void reset_generator() {
			delete generate_object_data;
			generate_object_data = new std::vector<XMLData*>();

			delete delete_object_data;
			delete_object_data = new std::vector<std::string>();
		}

	};

}

