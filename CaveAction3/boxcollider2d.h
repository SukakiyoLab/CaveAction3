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
        CAT_BoxCollider2D(CAT_Transform* transform, CAT_Rigidbody* rigidbody, unsigned short layer,
        double w, double h, Eigen::Vector2d offset,
        const float magnitude = DEFAULT_MAGNITUDE,
        const int collision = DEFAULT_COLLISION);
        void update();
        int judge(CAT_Collider2D *collider);

        Range get_range();
    };
}