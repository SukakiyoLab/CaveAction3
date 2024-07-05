#pragma once

#include "animation_image.h"
#include "rigidbody.h"
#include "animation_curve.h"


namespace component {

	class CAT_BallController : public CAT_Component{
	private:
		CAT_Transform* transform_ptr = nullptr;
		CAT_AnimationImage* animation_image_ptr = nullptr;
		CAT_Rigidbody* rigidbody_ptr = nullptr;
		CAT_AnimationCurve* animation_curve_ptr = nullptr;


		Eigen::Vector2i direction = {0,0};

	public:
		struct ComponentInitializer : CAT_Component::ComponentInitializer {
			unsigned short animation_image_id;
			unsigned short rigidbody_id;
			unsigned short animation_curve_id;
			Eigen::Vector2i direction = { 0,0 };
		};

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->animation_image_id = std::stoi(xmldata_ptr->nexts["animation_image_id"][0]->item);
			cInit_ptr->rigidbody_id = std::stoi(xmldata_ptr->nexts["rigidbody_id"][0]->item);
			cInit_ptr->animation_curve_id = std::stoi(xmldata_ptr->nexts["animation_curve_id"][0]->item);
			cInit_ptr->direction = Vector2i{ std::stoi(xmldata_ptr->nexts["direction"][0]->nexts["x"][0]->item), std::stoi(xmldata_ptr->nexts["direction"][0]->nexts["y"][0]->item) };
			
			return cInit_ptr;
		}


	public:
		void set_direction(Eigen::Vector2i new_direction);
		CAT_BallController(CAT_Transform* const transform_ptr,
			CAT_AnimationImage* const animation_image_ptr,
			CAT_Rigidbody* const rigidbody_ptr,
			CAT_AnimationCurve* const animation_curve_ptr,
			ComponentInitializer* const cInit);
		void update() override;

	};


}