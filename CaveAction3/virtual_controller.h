#pragma once
#include <eigen3/Eigen/Core>
#include "rigidbody.h"

#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#include "component.h"

#define CAT_INPUT_SPEED (10.0)
#define CAT_MAX_SPEED (100.0)

using namespace Eigen;

namespace component
{

	class CAT_VirtualController : public CAT_Component
	{
	private:
		float m_max_speed;
		float m_periodic_time = 0;
		CAT_Rigidbody *m_rigidbody;
		Vector3d m_real_input;
		Vector3d m_virtual_input;
		float m_hold_time = 0;
		float m_input_speed;

		float update_element(const float element_dest,
							 const float element_present,
							 const float delta_time);
		Vector3d limit(Vector3d velocity, Vector3d input);

	public:
		struct ComponentInitializer : public CAT_Component::ComponentInitializer {
			unsigned short rigidbody_id = 0;
			float input_speed = CAT_INPUT_SPEED;
			float max_speed = CAT_MAX_SPEED;
		};

	public:
		CAT_VirtualController(CAT_Rigidbody *const rigidbody, ComponentInitializer* const cInit);
		void set_max_speed(const float max_speed);
		void set_periodic_time(const float periodic_time);
		void input(const Vector3d input_force);
		void gain(const int delta_time) override;
	};
}