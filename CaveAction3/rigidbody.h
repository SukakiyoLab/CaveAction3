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

#include "xml_data.h"
#include "function_map.h"


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
		Vector3d m_velocity = Vector3d::Zero();
		double m_speed = -1; // 速度 //
		Vector3d m_acceleration = Vector3d::Zero();		 // 加速度 //
		float mass;						 // 質量 //
		Vector3d m_sum_force = Vector3d::Zero();			 // 力の総和 //
		Vector3d m_virtual_normal_force; // 仮想垂直抗力 //
		float friction_coefficient;				 // 動摩擦係数 //
		CAT_Transform* m_transform;

		float static_friction_magnitude = DEFAULT_STATIC_MAGNITUDE;
		int m_delta_time = 0;

		Vector3d pre_pos = Eigen::Vector3d::Zero();
		int pre_reset_time = 0;

	public:
		struct ComponentInitializer : public CAT_Component::ComponentInitializer {
			Type type = Newton;
			float mass = DEFAULT_MASS;
			float coefficient = DEFAULT_COEFFICIENT;
			Vector3d virtual_normal_force = Vector3d(DEFAULT_NORMAL_X, DEFAULT_NORMAL_Y, DEFAULT_NORMAL_Z);
		};

		static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
			ComponentInitializer* cInit_ptr = new ComponentInitializer;
			std::string type = xmldata_ptr->nexts["type"][0]->item;
			if (type == "Newton") {
				cInit_ptr->type = Type::Newton;
			}
			else {
				cInit_ptr->type = Type::Aristoteles;
			}
			cInit_ptr->mass = std::stof(xmldata_ptr->nexts["mass"][0]->item);
			cInit_ptr->coefficient = std::stof(xmldata_ptr->nexts["coefficient"][0]->item);
			cInit_ptr->virtual_normal_force = Eigen::Vector3d(std::stod(xmldata_ptr->nexts["virtual_normal_force"][0]->nexts["x"][0]->item),
				std::stod(xmldata_ptr->nexts["virtual_normal_force"][0]->nexts["y"][0]->item),
				std::stod(xmldata_ptr->nexts["virtual_normal_force"][0]->nexts["z"][0]->item));

			return cInit_ptr;
		}

	public:

		CAT_Rigidbody(CAT_Transform* const transform, ComponentInitializer* cInit);
		void gain(int delta_time) override;		/* 経過時間を取得 */
		void update() override; // 速度、加速度を計算し、物体の位置を更新する//
		int addForce(const Vector3d force); // 物体にかかる力を保存する//
		inline Vector3d get_velocity() {return this->m_velocity;}
		inline float get_mass() { return this->mass; }
		inline Vector3d get_sum_force() { return this->m_sum_force; }
		void reset_generator(); // 物体の速度を0にする//
		int set_velocity(const Vector3d velocity);
		Type get_type();
		inline void set_pre_pos() { pre_pos = this->m_transform->get_position(); }
		
		inline void set_speed(double new_speed) { if (this->type == Type::Aristoteles) this->m_speed = new_speed; };


		/* Address Getter */
		inline double* get_speed_ptr() { return &(this->m_speed); };
		

	};
}