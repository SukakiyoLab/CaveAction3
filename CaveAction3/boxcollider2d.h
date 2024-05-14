#pragma once
#include "collider2d.h"



namespace component
{

    class CAT_BoxCollider2D : public CAT_Collider2D
    {
    public:
        struct Range{
            double width;
            double height;

            Eigen::Vector2d offset;
            Eigen::Vector2d center;
        };

    
    private:
        double w;
        double h;
        Eigen::Vector2d offset;

        Range range;


    public:
        struct ComponentInitializer : public CAT_Collider2D::ComponentInitializer {
            double w = 1.0;
            double h = 1.0;
            Eigen::Vector2d offset = Eigen::Vector2d::Zero();
            unsigned short rigidbody_id = 0;
        };


    public:
        CAT_BoxCollider2D(CAT_Transform* transform, CAT_Rigidbody* rigidbody, ComponentInitializer* cInit);
        void update() override;
        int judge(CAT_Collider2D *collider);

        Range get_range();
    };
}