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

//#include "game_object.h"
#include "field.h"

#include "image.h"
#include "rigidbody.h"
#include "virtual_controller.h"

#include "shared_struct.h"

#include "AnimationEntity2D.h"
#include "Player2D.h"

#include "TilemapObject.h"

using namespace component;

class CAT_SDLEngine {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_GameController* m_controller = nullptr;
    ImageProjecter* m_projecter;
    ColliderManager* m_collider_manager;

    int count = 0;
	int quit = 0;
	SDL_Event e;

    enum TypeController {
        KEY_MOUSE,
        PAD_CONTROLLER
    };

    TypeController type_controller = PAD_CONTROLLER;



    CAT_Input input = {};

    Uint32 frameStart, frameTime;
    Uint32 preDeltaTime = 0;


    object::TilemapObject* field;
    object::TilemapObject* wall;
    object::TilemapObject* roof;


    object::Player2D* gb1;

    object::AnimationEntity2D* gb2;

private:

    void AddController(int index);
    void RemoveController();
    void InitInput();


public:
    int InitEngine();
    int InitObject();
    int Update();
    int Draw();
    int Finish();

    int Judge();
};