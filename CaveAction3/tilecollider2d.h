#pragma once

#include "collider2d.h"

#include <vector>


#define DEFAULT_EDGE_RATE 0.5

namespace component
{

    class CAT_TileCollider2D : public CAT_Collider2D
    {
    public:
        struct Line{
            std::pair<int, int>* first;
            std::pair<int, int>* second;
        };

    private:
        double m_width;

        std::vector<std::pair<int, int>*> points;

        std::vector<Line> lines;


    public:
        struct ComponentInitializer : public CAT_Collider2D::ComponentInitializer {
            double width;
            std::vector<std::vector<unsigned short>> tilemap_collider_init;
            int collision = 1;

            unsigned short rigidbody_id = 0;
        };
        

    public:
        CAT_TileCollider2D(CAT_Transform* transform, 
                        CAT_Rigidbody* rigidbody, 
                        ComponentInitializer* cInit);
        ~CAT_TileCollider2D();
        void update() override;
        int judge(CAT_Collider2D *collider);

        

    };
}