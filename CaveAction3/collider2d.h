#pragma once

#include <eigen3/Eigen/Dense>



#include "transform.h"
#include "rigidbody.h"
#include "component.h"

#define DEFAULT_MAGNITUDE (4500)
#define DEFAULT_COLLISION (1)

#define DEFAULT_MAX_COLLISION_LAYER (63)

namespace component
{

    class CAT_Collider2D : public CAT_Component
    {
    protected:
        unsigned short m_layer;
        CAT_Transform* m_transform;
        CAT_Rigidbody* m_rigidbody;
        float m_force_magnitude;
        int m_collision;

    public:
        struct ComponentInitializer : public CAT_Component::ComponentInitializer {
            unsigned short layer = 0;
            float magnitude = DEFAULT_MAGNITUDE;
            int collision = DEFAULT_COLLISION;
        };

        

    public:
        CAT_Collider2D(CAT_Transform* transform, 
                        CAT_Rigidbody* const rigidbody, 
                        ComponentInitializer* cInit);
        virtual int judge(CAT_Collider2D *collider);
        void update() override;
        unsigned short get_layer();
        int get_collision();
        void add_force(Eigen::Vector3d force);
        Eigen::Vector3d get_velocity();
        float get_mass();
        Eigen::Vector3d get_sum_force();
        void reset_generator();
    };

}
