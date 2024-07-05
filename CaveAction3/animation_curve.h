#pragma once

#include "component.h"
#include "animation_root.h"



#include <vector>

#include "xml_data.h"
#include "function_map.h"

#include <vector>

#include "transform.h"

namespace component {
	
	class CAT_AnimationCurve : public CAT_Component, CAT_AnimationRoot {
	private:
		std::vector<std::pair<int, double>> key_points;
		int loop = 0;
		double* set_ptr = nullptr;
		std::map<int, double> frame_data;
		int time = 0;
		int sum_time = 0;

	public:
		struct ComponentInitializer : CAT_Component::ComponentInitializer{
			XMLData* key_points_data = nullptr;
			int loop = 0;
		};

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			
			cInit_ptr->key_points_data = funcMap_ptr->use_char_func(xmldata_ptr->nexts["key_points_data"][0]->item);
			cInit_ptr->loop = std::stoi(xmldata_ptr->nexts["loop"][0]->item);

			return cInit_ptr;

		}

	private:
		int factorial(int n);
		int biCoe(int n, int k);
		float bernstein(int n, int i, float t);
		float bezierCurve(std::vector<float> points, float t);

	public:
		CAT_AnimationCurve(ComponentInitializer* cInit_ptr);
		void gain(int delta_time) override;
		void update() override;
		void reset();
		inline void regist_address(double * set_ptr) { this->set_ptr = set_ptr; };
	};

}