#pragma once

#include "collider2d.h"

#include <vector>

#include "xml_data.h"
#include "function_map.h"

#include <eigen3/Eigen/Dense>

#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY


#define DEFAULT_EDGE_RATE 1.0

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

        Eigen::Vector2i m_offset;


    public:
        struct ComponentInitializer : public CAT_Collider2D::ComponentInitializer {
            double width;
            std::vector<std::vector<unsigned short>>* tilemap_collider_init;
            //int collision = 1;
            Eigen::Vector2i offset = Eigen::Vector2i::Zero();

            unsigned short rigidbody_id = 0;
        };

        static ComponentInitializer* create_initializer(XMLData* xmldata_ptr, FunctionMap* funcMap_ptr) {
            ComponentInitializer* cInit_ptr = new ComponentInitializer;
            cInit_ptr->layer = std::stoi(xmldata_ptr->nexts["layer"][0]->item);
            cInit_ptr->magnitude = std::stof(xmldata_ptr->nexts["magnitude"][0]->item);
            cInit_ptr->collision = std::stoi(xmldata_ptr->nexts["collision"][0]->item);
            cInit_ptr->width = std::stod(xmldata_ptr->nexts["width"][0]->item);
            cInit_ptr->tilemap_collider_init = funcMap_ptr->use_value_func(xmldata_ptr->nexts["tilemap_collider_init"][0]->item);
            cInit_ptr->offset = Eigen::Vector2i(std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["x"][0]->item), std::stoi(xmldata_ptr->nexts["offset"][0]->nexts["y"][0]->item));
            cInit_ptr->rigidbody_id = std::stoi(xmldata_ptr->nexts["rigidbody_id"][0]->item);

            return cInit_ptr;
        }
        

    public:
        CAT_TileCollider2D(CAT_Transform* transform, 
                        CAT_Rigidbody* rigidbody, 
                        ComponentInitializer* cInit);
        ~CAT_TileCollider2D();
        void update() override;
        int judge(CAT_Collider2D *collider);

        

    };
}