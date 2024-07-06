#pragma once
#include "collider2d.h"

#include "xml_data.h"
#include "function_map.h"

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

        int overlap = 0;


    public:
        struct ComponentInitializer : public CAT_Collider2D::ComponentInitializer {
            double w = 1.0;
            double h = 1.0;
            Eigen::Vector2d offset = Eigen::Vector2d::Zero();
            unsigned short rigidbody_id = 0;
        };

        static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
            ComponentInitializer* cInit_ptr = new ComponentInitializer;
            cInit_ptr->layer = std::stoi(xmldata_ptr->nexts["layer"][0]->item);
            cInit_ptr->magnitude = std::stof(xmldata_ptr->nexts["magnitude"][0]->item);
            cInit_ptr->collision = std::stoi(xmldata_ptr->nexts["collision"][0]->item);
            cInit_ptr->w = std::stod(xmldata_ptr->nexts["w"][0]->item);
            cInit_ptr->h = std::stod(xmldata_ptr->nexts["h"][0]->item);
            cInit_ptr->offset = Eigen::Vector2d{std::stod(xmldata_ptr->nexts["offset"][0]->nexts["x"][0]->item),std::stod(xmldata_ptr->nexts["offset"][0]->nexts["y"][0]->item)};
            cInit_ptr->rigidbody_id = std::stoi(xmldata_ptr->nexts["rigidbody_id"][0]->item);

            return cInit_ptr;
        }


    public:
        CAT_BoxCollider2D(CAT_Transform* transform, CAT_Rigidbody* rigidbody, ComponentInitializer* cInit);
        void update() override;
        int judge(CAT_Collider2D *collider);

        Range get_range();
    };
}