#include "collider2d.h"

#include "debug_log.h"

namespace component
{

    CAT_Collider2D::CAT_Collider2D(CAT_Transform* transform, 
                                    CAT_Rigidbody* rigidbody, 
                                    CAT_Collider2D::ComponentInitializer* cInit){
        this->m_transform = transform;
        this->m_rigidbody = rigidbody;
        this->m_layer = cInit->layer;
        this->m_force_magnitude = cInit->magnitude;
        this->m_collision = cInit->collision;
    }

    unsigned short CAT_Collider2D::get_layer()
    {
        return m_layer;
    }

    void CAT_Collider2D::update(){

    }

    int CAT_Collider2D::judge(CAT_Collider2D* collider){
        return 0;
    }

    int CAT_Collider2D::get_collision(){
        return m_collision;
    }

    void CAT_Collider2D::add_force(Eigen::Vector3d force){
        if (this->m_rigidbody != nullptr) {
            this->m_rigidbody->addForce(force);
        }
        else {
            debug::debugLog("This is static Collider!");
        }
    }

    Eigen::Vector3d CAT_Collider2D::get_velocity(){
        return this->m_rigidbody->get_velocity();
    }
}