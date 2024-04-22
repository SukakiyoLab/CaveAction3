#include "boxcollider2d.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "debug_log.h"



namespace component
{

    CAT_BoxCollider2D::CAT_BoxCollider2D(CAT_Transform* transform,
                                            CAT_Rigidbody* rigidbody,
                                            unsigned short layer,
                                            double w, 
                                            double h, 
                                            Eigen::Vector2d offset,
                                            const float magnitude,
                                            const int collision)
        : CAT_Collider2D(transform, rigidbody, layer, magnitude, collision)
    {
        this->w = w;
        this->h = h;
        this->offset = offset;

        this->update();

    }

    void CAT_BoxCollider2D::update(){
        Eigen::Vector3d scale = m_transform->get_scale();
        Eigen::Vector3d pos = m_transform->get_position();
            
        range.width = w * scale[0];
        range.height = h * scale[1];

        range.offset = offset;
        range.center = Eigen::Vector2d(pos[0], pos[1]) + offset;
        
    }

    int CAT_BoxCollider2D::judge(CAT_Collider2D *collider)
    {

        CAT_BoxCollider2D *box_col = dynamic_cast<CAT_BoxCollider2D *>(collider);
        if (box_col != nullptr)
        {
            Range target_range = box_col->get_range();
            Vector2d dist_vec = target_range.center - range.center;
            if(abs(dist_vec[0]) < (range.width + target_range.width) / 2 &&
               abs(dist_vec[1]) < (range.height + target_range.height) / 2){
                debug::debugLog("Collider On\n");
                if(collider->get_collision() > 0){
                    Vector3d vec = Vector3d::Zero();
                    Vector3d relative_velocity = collider->get_velocity() - this->get_velocity();
                    if(Vector3d(dist_vec[0], dist_vec[1], 0).dot(relative_velocity) < 0){
                        vec = 3000 * Vector3d(dist_vec[0], dist_vec[1], 0).normalized()
                                                    - 80 * relative_velocity;
                    }
                    collider->add_force(vec);
                }
                return 1;
            }
            
            
        }

        return 0;
    }


    CAT_BoxCollider2D::Range CAT_BoxCollider2D::get_range(){
        return this->range;
    }
}