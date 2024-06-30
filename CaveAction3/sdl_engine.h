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

#include "GameObject.h"

#include "NavMeshEntity.h"

#include "function_map.h"

#include "object_manager.h"
#include "object_generator.h"

using namespace component;

class CAT_SDLEngine {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_GameController* m_controller = nullptr;
    ImageProjecter* m_projecter;
    ColliderManager* m_collider_manager;

    NavMeshSystem2D* m_nav_mesh_sys;
    FunctionMap* functionMap;
    
    game_system_1::ObjectManager* object_manager;
    game_system_3::ObjectGenerator* object_generator;

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

    object::GameObject* field;
    object::GameObject* field2;
    object::GameObject* field3;

    object::GameObject* wall;
    object::GameObject* wall2;
    object::GameObject* wall3;

    object::GameObject* roof;
    object::GameObject* roof2;
    object::GameObject* roof3;


    //object::Player2D* gb1;

    object::GameObject* gb2;

    object::GameObject* camera;

    //object::Slime2D* gb3;

    object::GameObject* playerObj;

    object::GameObject* slimeObj;

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