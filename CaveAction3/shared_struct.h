#pragma once

#include <eigen3/Eigen/Core>
#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY

struct CAT_Input {

    

    int right = 0;
    int left = 0;
    int front = 0;
    int back = 0;

    int right_trigger = 0;
    bool right_trigger_switch = false;

    int escape = 0;
};

struct CAT_NMRect {

    unsigned short id = 255;

    int top = 0;
    int left = 0;
    int right = 0;
    int bottom = 0;

    Eigen::Vector2i center;


    std::vector<unsigned short> next_id;
};