#pragma once

#include <vector>
#include <pugixml.hpp>
#include <cstdlib>

#include <string>

#include "shared_struct.h"
#include "debug_log.h"

#include <map>

namespace XMLLoader {

	class AnimLoader {
	private:
		struct Animation {
			std::vector<std::string> filenames;
			std::vector<double> durations;
		};

		std::vector<Animation> animations;

	public:
		void Load(const char* xml_file);

		std::vector<std::string> get_filenames(unsigned short id) { return animations[id].filenames; }
		std::vector<double> get_durations(unsigned short id) { return animations[id].durations; }
		int get_size() { return animations.size(); }


	};


	std::vector<CAT_NMRect> NMRectLoad(const char* rect_data, Eigen::Vector2i offset);
}