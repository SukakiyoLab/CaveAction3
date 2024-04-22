#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>


#include <eigen3/Eigen/Core>
#define EIGEN_NO_DEBUG 
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DONT_PARALLELIZE
#define EIGEN_MPL2_ONLY


#include "debug_log.h"
#include "image_storage.h"
#include "image_projecter.h"
#include "collider_manager.h"

#include "nav_mesh_system2d.h"

//#include "game_object.h"
#include "field.h"

#include "image.h"
#include "rigidbody.h"
#include "virtual_controller.h"

#include "shared_struct.h"

#include "CameraObject.h"
#include "AnimationEntity2D.h"
#include "Player2D.h"

#include "Slime2D.h"

#include "TilemapObject.h"

#include "NavMeshEntity.h"

using namespace component;

class CAT_SDLEngine {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_GameController* m_controller = nullptr;
    ImageProjecter* m_projecter;
    ColliderManager* m_collider_manager;

    NavMeshSystem2D* m_nav_mesh_sys;
    

    int count = 0;
	int quit = 0;
	SDL_Event e;
    Eigen::Vector3i window_pos = {};

    enum TypeController {
        KEY_MOUSE,
        PAD_CONTROLLER
    };

    TypeController type_controller = PAD_CONTROLLER;



    CAT_Input input = {};

    Uint32 frameStart, frameTime;
    Uint32 preDeltaTime = 0;

    
    object::NavMeshEntity* nav_mesh_entity;

    object::TilemapObject* field;
    object::TilemapObject* field2;
    object::TilemapObject* field3;

    object::TilemapObject* wall;
    object::TilemapObject* wall2;
    object::TilemapObject* wall3;

    object::TilemapObject* roof;
    object::TilemapObject* roof2;
    object::TilemapObject* roof3;


    object::Player2D* gb1;

    object::AnimationEntity2D* gb2;

    object::CameraObject* camera;

    object::Slime2D* gb3;

private:

    void AddController(int index);
    void RemoveController();
    void InitInput();
    bool CheckWindowPosition();
    void InitWindowPosition();

public:
    int InitEngine();
    int InitObject();
    int Update();
    int Draw();
    int Finish();

    int Judge();
};