#include "tilecollider2d.h"

#include "csv_loader.h"

#include <map>

#include "debug_log.h"

#include "boxcollider2d.h"

#include <cmath>

#define VELOCITY_COF (70)

#define POSITION_COF (150)
#define FORCE_COF (-1)

namespace component
{

    CAT_TileCollider2D::CAT_TileCollider2D(CAT_Transform* transform, 
                        CAT_Rigidbody* rigidbody, 
                        ComponentInitializer* cInit)
    :CAT_Collider2D(transform, rigidbody, static_cast<CAT_Collider2D::ComponentInitializer*>(cInit))                    
    {
        this->m_width = cInit->width;

        this->m_collision = cInit->collision;


        //std::vector<std::vector<unsigned short>> csv_file = CSV_Load(std::string(file_name));
        std::vector<std::vector<unsigned short>>* csv_file = cInit->tilemap_collider_init;


        int h_num = csv_file->size() + 2;
        int w_num = 0;
        if(h_num > 2){
            w_num = csv_file->at(0).size() + 2;
        }

        std::vector<unsigned short> field(w_num * h_num);

        std::fill(field.begin(), field.end(), -1);

        for(int i = 1; i < h_num - 1; i++){
            for(int j = 1; j < w_num - 1; j++){
                field[i * w_num + j] = (*csv_file)[i-1][j-1];
            }
        }

        struct Point{
            std::pair<int,int>* pos = nullptr;
            int front = 0;
            int back = 0;
            int left = 0;
            int right = 0;
        };

        std::map<std::pair<int,int>,Point> point_set;

        for(int i = 1; i < h_num - 1; i++){
            for(int j = 1; j < w_num - 1; j++){
                if(field[i * w_num + j] == 1){
                    int front = 0;
                    int back = 0;
                    int left = 0;
                    int right = 0;
                    if(field[(i - 1) * w_num + j] == 1){
                        front = 1;
                    }
                    if(field[(i + 1) * w_num + j] == 1){
                        back = 1;
                    }
                    if(field[i * w_num + (j - 1)] == 1){
                        left = 1;
                    }
                    if(field[i * w_num + (j + 1)] == 1){
                        right = 1;
                    }

                    if((front - back) != 0 || (right - left) != 0){
                        Point point = Point{new std::pair<int,int>(j-1, i-1), front, back, left, right};
                        point_set[*(point.pos)] = point;
                        points.push_back(point.pos);
                    }
                    
                }
            }
        }

        for(int i = 0; i < points.size(); i++){
            Point* point = &(point_set[*(points[i])]);
            Point* front_target = nullptr;
            Point* back_target = nullptr;
            Point* left_target = nullptr;
            Point* right_target = nullptr;
            for(int j = 0; j < points.size(); j++) {
                if(j != i){
                    if(points[i]->first == points[j]->first){
                        if(point->back > 0 && points[i]->second < points[j]->second){
                            if(back_target == nullptr){
                                back_target = &(point_set[*(points[j])]);
                            }else if(points[j]->second < back_target->pos->second){
                                back_target = &(point_set[*(points[j])]);
                            }
                        }

                        if(point->front > 0 && points[i]->second > points[j]->second){
                            if(front_target == nullptr){
                                front_target = &(point_set[*(points[j])]);
                            }else if(points[j]->second > front_target->pos->second){
                                front_target = &(point_set[*(points[j])]);
                            }
                        }
                    }

                    if(points[i]->second == points[j]->second){
                        if(point->left > 0 && points[i]->first > points[j]->first){
                            if(left_target == nullptr){
                                left_target = &(point_set[*(points[j])]);
                            }else if(points[j]->first > left_target->pos->first){
                                left_target = &(point_set[*(points[j])]);
                            }
                        }

                        if(point->right > 0 && points[i]->first < points[j]->first){
                            if(right_target == nullptr){
                                right_target = &(point_set[*(points[j])]);
                            }else if(points[j]->first < right_target->pos->first){
                                right_target = &(point_set[*(points[j])]);
                            }
                        }
                    }
                }
            }

            if(point->back > 0 && back_target != nullptr){
                lines.push_back(Line{point->pos, back_target->pos});

                point->back = 0;
                back_target->front = 0;
            }

            if(point->front > 0 && front_target != nullptr){
                lines.push_back(Line{front_target->pos, point->pos});

                point->front = 0;
                front_target->back = 0;
            }

            if(point->right > 0 && right_target != nullptr){
                lines.push_back(Line{point->pos, right_target->pos});

                point->right = 0;
                right_target->left = 0;
            }

            if(point->left > 0 && left_target != nullptr){
                lines.push_back(Line{left_target->pos, point->pos});

                point->left = 0;
                left_target->right = 0;
            }
        }

        for(int i = 0 ; i < lines.size(); i++){
            debug::debugLog("Lines1 x: %d y: %d\n", lines[i].first->first, lines[i].first->second);
            debug::debugLog("Lines2 x: %d y: %d\n", lines[i].second->first, lines[i].second->second);
        }
    }

    CAT_TileCollider2D::~CAT_TileCollider2D(){
        for(int i = 0; i < points.size(); i++){
            delete points[i];
        }
    }

    int CAT_TileCollider2D::judge(CAT_Collider2D *collider){
        CAT_BoxCollider2D* box_col = dynamic_cast<CAT_BoxCollider2D*>(collider);

        int result = 0;

        if(box_col != nullptr){

            Vector3d force = Vector3d::Zero();
            int force_num = 0;

            for(int i = 0 ; i < lines.size(); i++){
                Line line = lines[i];
                int type = 0;
                if(line.first->first == line.second->first){
                    type = 0;
                }else if(line.first->second == line.second->second){
                    type = 1;
                }

                Eigen::Vector2d own_pos = Eigen::Vector2d((double)this->m_transform->get_position()[0],(double)this->m_transform->get_position()[1]) + this->m_offset.cast<double>();
                Eigen::Vector2d edge1(line.first->first * 32 + 16 + own_pos[0], line.first->second * 32 + 16 + own_pos[1]);
                Eigen::Vector2d edge2(line.second->first * 32 + 16 + own_pos[0], line.second->second * 32 + 16 + own_pos[1]);
                CAT_BoxCollider2D::Range range = box_col->get_range();
                Eigen::Vector2d pos = range.center;
                double height = range.height;
                double width = range.width;




                if(type == 0){  /* ècí∑Collider */

                    double sum_width = width / 2 + m_width / 2;
                    Vector2d dist_vec = pos - Vector2d(edge1[0], pos[1]);
                    Eigen::Vector3d vec = Eigen::Vector3d::Zero();

                    if(edge1[1] < pos[1] + height / 2
                    && edge2[1] > pos[1] - height / 2){ /* ècí∑Collider : íÜâõ */
                        
                        if(dist_vec.norm() < sum_width){
                            debug::debugLog("Collider On\n");

                            if (pos[0] < edge1[0]) {
                                dist_vec = pos - Vector2d(edge1[0] - this->m_width / 2, pos[1]);
                            }
                            else {
                                dist_vec = pos - Vector2d(edge1[0] + this->m_width / 2, pos[1]);
                            }

                            if (pos[0] < edge1[0]) {
                                dist_vec = Vector2d(edge1[0] - pos[0] - (width / 2 + this->m_width / 2), 0);
                            }
                            else {
                                dist_vec = Vector2d(edge1[0] - pos[0] + (width / 2 + this->m_width / 2), 0);
                            }

                            Vector3d dist_vec_3d = Vector3d(dist_vec[0], dist_vec[1], 0);
                            if (dist_vec_3d.dot(collider->get_velocity()) < 0) {
                                vec = VELOCITY_COF * abs(collider->get_velocity()[0]) * dist_vec_3d.normalized();
                                vec += FORCE_COF * collider->get_sum_force();
                            }
                            if (dist_vec_3d.dot(collider->get_sum_force()) < -0.1) {
                                
                            }
                            
                            vec += POSITION_COF * dist_vec_3d;
                            
                            

                            if(collider->get_collision() > 0){
                                force += vec;
                                if (force != Vector3d::Zero()) {
                                    force_num++;
                                }
                            }
                            result = 1;
                        } 
                    }
                    else if (edge1[1] - this->m_width / 2 * DEFAULT_EDGE_RATE < pos[1] + height / 2
                         && pos[1] + height / 2 < edge1[1]
                        //(edge1 - pos).norm() < this->m_width / 2
                    ){                                  
                        if(dist_vec.norm() < sum_width){
                            debug::debugLog("Collider On\n");
                            
                            dist_vec = (edge1 - pos);
                            int key = 0;

                            if (abs(dist_vec[1] / dist_vec[0]) < abs((this->m_width + height) / (this->m_width + width))) {
                                key = 0;
                            }
                            else {
                                key = 1;
                            }


                            if (key == 0) {
                                double sum = this->m_width / 2 + width / 2;
                                if (dist_vec[0] > 0) {
                                    dist_vec = Vector2d((abs(dist_vec[0]) - sum), 0);
                                }
                                else {
                                    dist_vec = Vector2d((sum - abs(dist_vec[0])), 0);
                                }
                            }
                            else {
                                double sum = this->m_width / 2  + height / 2;
                                dist_vec = Vector2d(0, (abs(dist_vec[1]) - sum));
                            }

                            Vector3d dist_vec_3d = Vector3d(dist_vec[0], dist_vec[1], 0);
                            if (dist_vec_3d.dot(collider->get_velocity()) < 0) {
                                vec = VELOCITY_COF * abs(collider->get_velocity()[key]) * dist_vec_3d.normalized();
                                vec += FORCE_COF * collider->get_sum_force();
                            }
                            if (dist_vec_3d.dot(collider->get_sum_force()) < -0.1) {
                                
                            }
                            vec += POSITION_COF * dist_vec_3d;

                            if(collider->get_collision() > 0){
                                force += vec;
                                if (force != Vector3d::Zero()) {
                                    force_num++;
                                }
                            }
                            result = 1;
                        } 
                    }
                    else if(edge2[1] < pos[1] - height / 2
                         && pos[1] - height / 2 < edge2[1] + this->m_width / 2 * DEFAULT_EDGE_RATE
                        //(edge2 - pos).norm() < this->m_width / 2
                    ){                                  
                        if(dist_vec.norm() < sum_width){
                            debug::debugLog("Collider On\n");
                            
                            dist_vec = (edge2 - pos);
                            int key = 0;

                            if (abs(dist_vec[1] / dist_vec[0]) < abs((this->m_width + height) / (this->m_width + width))) {
                                key = 0;
                            }
                            else {
                                key = 1;
                            }


                            if (key == 0) {
                                double sum = this->m_width / 2 + width / 2;
                                if (dist_vec[0] > 0) {
                                    dist_vec = Vector2d((abs(dist_vec[0]) - sum), 0);
                                }
                                else {
                                    dist_vec = Vector2d((sum - abs(dist_vec[0])), 0);
                                }
                            }
                            else {
                                double sum = this->m_width / 2 + height / 2;
                                dist_vec = Vector2d(0, (sum - abs(dist_vec[1])));
                            }

                            Vector3d dist_vec_3d = Vector3d(dist_vec[0], dist_vec[1], 0);
                            if (dist_vec_3d.dot(collider->get_velocity()) < 0) {
                                vec = VELOCITY_COF * abs(collider->get_velocity()[key]) * dist_vec_3d.normalized();
                                vec += FORCE_COF * collider->get_sum_force();
                            }
                            if (dist_vec_3d.dot(collider->get_sum_force()) < -0.1) {
                                
                            }
                            vec += POSITION_COF * dist_vec_3d;

                            if (collider->get_collision() > 0) {
                                force += vec;
                                if (force != Vector3d::Zero()) {
                                    force_num++;
                                }
                            }
                            result = 1;
                        } 
                    }
                    
                }

                if(type == 1){ /* â°í∑Collider */

                    double sum_width = height / 2 + m_width / 2;
                    Vector2d dist_vec = pos - Vector2d(pos[0], edge1[1]);
                    Eigen::Vector3d vec = Eigen::Vector3d::Zero();

                    if(edge1[0] < pos[0] + width / 2
                    && edge2[0] > pos[0] - width / 2){ /* â°í∑Collider : íÜâõïî */
                        
                        if(dist_vec.norm() < sum_width){
                            debug::debugLog("Collider On\n");
                            
                            if (pos[1] < edge1[1]) {
                                dist_vec = - Vector2d(pos[0], pos[1] + height / 2) + Vector2d(pos[0], edge1[1] - this->m_width / 2);
                            }
                            else {
                                dist_vec = - Vector2d(pos[0], pos[1] - height / 2) + Vector2d(pos[0], edge1[1] + this->m_width / 2);
                            }


                            Vector3d dist_vec_3d = Vector3d(dist_vec[0], dist_vec[1], 0);
                            if (dist_vec_3d.dot(collider->get_velocity()) < 0) {
                                vec = VELOCITY_COF * abs(collider->get_velocity()[1]) * dist_vec_3d.normalized();
                                vec += FORCE_COF * collider->get_sum_force();
                            }
                            if (dist_vec_3d.dot(collider->get_sum_force()) < -0.1) {
                                
                            }
                            vec += POSITION_COF * dist_vec_3d;

                            if(collider->get_collision() > 0){
                                force += vec;
                                if (force != Vector3d::Zero()) {
                                    force_num++;
                                }
                            }
                            result = 1;
                        } 
                    }
                    else if (edge1[0] - this->m_width / 2 * DEFAULT_EDGE_RATE < pos[0] + width / 2
                         && pos[0] + width / 2 < edge1[0]
                        //(edge1 - pos).norm() < this->m_width / 2
                    ){                                  
                        if(dist_vec.norm() < sum_width){
                            dist_vec = (edge1 - pos);
                            int key = 0;

                            if (abs(dist_vec[1] / dist_vec[0]) < abs((this->m_width + height) / (this->m_width + width))) {
                                key = 0;
                            }
                            else {
                                key = 1;
                            }


                            if (key == 0) {
                                double sum = this->m_width / 2 + width / 2;
                                dist_vec = Vector2d((abs(dist_vec[0]) - sum), 0);
                                
                            }
                            else {
                                double sum = this->m_width / 2 + height / 2;
                                if (dist_vec[1] > 0) {
                                    dist_vec = Vector2d(0,(abs(dist_vec[1]) - sum));
                                }
                                else {
                                    dist_vec = Vector2d(0,(sum - abs(dist_vec[1])));
                                }
                            }

                            Vector3d dist_vec_3d = Vector3d(dist_vec[0], dist_vec[1], 0);
                            if (dist_vec_3d.dot(collider->get_velocity()) < 0) {
                                vec = VELOCITY_COF * abs(collider->get_velocity()[key]) * dist_vec_3d.normalized();
                                vec += FORCE_COF * collider->get_sum_force();
                            }
                            if (dist_vec_3d.dot(collider->get_sum_force()) < -0.1) {
                                
                            }
                            vec += POSITION_COF * dist_vec_3d;

                            if (collider->get_collision() > 0) {
                                force += vec;
                                if (force != Vector3d::Zero()) {
                                    force_num++;
                                }
                            }
                            result = 1;
                        } 
                    }
                    else if(edge2[0] < pos[0] - width / 2
                         && pos[0] - width / 2 < edge2[0] + this->m_width / 2 * DEFAULT_EDGE_RATE
                        //(edge2 - pos).norm() < this->m_width / 2
                    ){                                  
                        if(dist_vec.norm() < sum_width){
                            debug::debugLog("Collider On\n");
                            
                            dist_vec = (edge2 - pos);
                            int key = 0;

                            if (abs(dist_vec[1] / dist_vec[0]) < abs((this->m_width + height) / (this->m_width + width))) {
                                key = 0;
                            }
                            else {
                                key = 1;
                            }


                            if (key == 0) {
                                double sum = this->m_width / 2 + width / 2;
                                dist_vec = Vector2d((sum - abs(dist_vec[0])), 0);

                            }
                            else {
                                double sum = this->m_width / 2 + height / 2;
                                if (dist_vec[1] > 0) {
                                    dist_vec = Vector2d(0, (abs(dist_vec[1]) - sum));
                                }
                                else {
                                    dist_vec = Vector2d(0, (sum - abs(dist_vec[1])));
                                }
                            }

                            Vector3d dist_vec_3d = Vector3d(dist_vec[0], dist_vec[1], 0);
                            if (dist_vec_3d.dot(collider->get_velocity()) < 0) {
                                vec = VELOCITY_COF * abs(collider->get_velocity()[key]) * dist_vec_3d.normalized();
                                vec += FORCE_COF * collider->get_sum_force();
                            }
                            if (dist_vec_3d.dot(collider->get_sum_force()) < -0.1) {
                                
                            }
                            vec += POSITION_COF * dist_vec_3d;

                            if (collider->get_collision() > 0) {
                                force += vec;
                                if (force != Vector3d::Zero()) {
                                    force_num++;
                                }
                            }
                            result = 1;
                        } 
                    }
                }
            }


            if (collider->get_collision() > 0) {
                /*if (force_num != 0) {
                    collider->add_force(force);
                }
                */
                if (result > 0) collider->reset_generator();
            }

        }

        return result;
    }

    void CAT_TileCollider2D::update(){

    }

}