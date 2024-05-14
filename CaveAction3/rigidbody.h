#pragma once
#include <eigen3/Eigen/Dense>

#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

#include <list>

#include "transform.h"

#include <cmath>

#define DEFAULT_MASS (1.0)
#define DEFAULT_COEFFICIENT (1.0)

#define DEFAULT_STATIC_MAGNITUDE (1000)
#define DEFAULT_NORMAL_X (0)
#define DEFAULT_NORMAL_Y (0)
#define DEFAULT_NORMAL_Z (1000)

#define DEFAULT_STATIC_VELOCITY (10)

#include "component.h"


using namespace Eigen;
//using namespace std;

namespace component
{

	
	class CAT_Rigidbody : public CAT_Component
	{
	public:
		typedef enum
		{
			Newton,
			Aristoteles

		} Type;


	private:
		Type type;
		Vector3d m_velocity = Vector3d::Zero();			 // 速度 //
		Vector3d m_acceleration = Vector3d::Zero();		 // 加速度 //
		float mass;						 // 質量 //
		Vector3d m_sum_force = Vector3d::Zero();			 // 力の総和 //
		Vector3d m_virtual_normal_force; // 仮想垂直抗力 //
		float friction_coefficient;				 // 動摩擦係数 //
		CAT_Transform* m_transform;

		float static_friction_magnitude = DEFAULT_STATIC_MAGNITUDE;

	public:
		struct ComponentInitializer : public CAT_Component::ComponentInitializer {
			Type type = Newton;
			float mass = DEFAULT_MASS;
			float coefficient = DEFAULT_COEFFICIENT;
			Vector3d virtual_normal_force = Vector3d(DEFAULT_NORMAL_X, DEFAULT_NORMAL_Y, DEFAULT_NORMAL_Z);
		};

	public:

		CAT_Rigidbody(CAT_Transform* const transform, ComponentInitializer* cInit);
		void gain(int delta_time) override;		// 速度、加速度を計算し、物体の位置を更新する//
		int addForce(const Vector3d force); // 物体にかかる力を保存する//
		Vector3d get_velocity() {return this->m_velocity;}
		void reset(); // 物体の速度を0にする//
		int set_velocity(const Vector3d velocity);
		Type get_type();
	};
}