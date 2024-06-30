#pragma once
#include <eigen3/Eigen/Core>

#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY
#include <cstdio>

#include "component.h"

#include "xml_data.h"
#include "function_map.h"

using namespace Eigen;

namespace component {

	class CAT_Transform : public CAT_Component {
	private:
		Vector3d m_position = { 0, 0, 0 }; // ˆÊ’u
		Vector3d m_rotation = { 0, 0, 0 };
		Vector3d m_scale    = { 1, 1, 1 };

	public:
		struct ComponentInitializer : public CAT_Component::ComponentInitializer {
			Vector3d position = Eigen::Vector3d(0, 0, 0);
			Vector3d rotation = Eigen::Vector3d(0, 0, 0);
			Vector3d scale = Eigen::Vector3d(1, 1, 1);
		};

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			cInit_ptr->position = Eigen::Vector3d(std::stod(xmldata_ptr->nexts["position"][0]->nexts["x"][0]->item),
				std::stod(xmldata_ptr->nexts["position"][0]->nexts["y"][0]->item),
				std::stod(xmldata_ptr->nexts["position"][0]->nexts["z"][0]->item));
			cInit_ptr->rotation = Eigen::Vector3d(std::stod(xmldata_ptr->nexts["rotation"][0]->nexts["x"][0]->item),
				std::stod(xmldata_ptr->nexts["rotation"][0]->nexts["y"][0]->item),
				std::stod(xmldata_ptr->nexts["rotation"][0]->nexts["z"][0]->item));
			cInit_ptr->scale = Eigen::Vector3d(std::stod(xmldata_ptr->nexts["scale"][0]->nexts["x"][0]->item),
				std::stod(xmldata_ptr->nexts["scale"][0]->nexts["y"][0]->item),
				std::stod(xmldata_ptr->nexts["scale"][0]->nexts["z"][0]->item));

			return cInit_ptr;
		}


	public:
		CAT_Transform(ComponentInitializer* cInit);
		~CAT_Transform();
		void set_position(const Vector3d vec) { m_position = vec; };
		void set_rotation(const Vector3d vec) { m_rotation = vec; };
		void set_scale(const Vector3d vec) { m_scale = vec; };
		Vector3d get_position() { return m_position; };
		Vector3d get_rotation() { return m_rotation; };
		Vector3d get_scale() { return m_scale; }
	};
}
