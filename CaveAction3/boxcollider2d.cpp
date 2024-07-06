#include "boxcollider2d.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "debug_log.h"



namespace component
{

    CAT_BoxCollider2D::CAT_BoxCollider2D(CAT_Transform* transform,
                                            CAT_Rigidbody* rigidbody,
                                            ComponentInitializer* cInit)
        : CAT_Collider2D(transform, rigidbody, static_cast<CAT_Collider2D::ComponentInitializer*>(cInit))
    {
        this->w = cInit->w;
        this->h = cInit->h;
        this->offset = cInit->offset;

        this->update();

    }

    void CAT_BoxCollider2D::update(){
        Eigen::Vector3d scale = m_transform->get_scale();
        Eigen::Vector3d pos = m_transform->get_position();
            
        range.width = w * scale[0];
        range.height = h * scale[1];

        range.offset = offset;
        range.center = Eigen::Vector2d(pos[0], pos[1]) + offset;

        if (this->overlap > 0) {
            
        }
        this->overlap = 0;
        
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
                //debug::debugLog("Collider On\n");
                if(collider->get_collision() > 0){
                    Vector3d vec = Vector3d::Zero();
                    Vector3d relative_velocity = collider->get_velocity() - this->get_velocity();
                    if(Vector3d(dist_vec[0], dist_vec[1], 0).dot(relative_velocity) < 0){
                        vec = 3000 * Vector3d(dist_vec[0], dist_vec[1], 0).normalized()
                                                    - 80 * relative_velocity;
                    }
                    //collider->add_force(vec);
                }
                this->overlap = 1;
                return 1;
            }
            
            
        }
        this->m_rigidbody->set_pre_pos();
        return 0;
    }


    CAT_BoxCollider2D::Range CAT_BoxCollider2D::get_range(){
        return this->range;
    }
}