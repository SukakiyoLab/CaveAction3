#include "sdl_engine.h"

#ifdef _DEBUG
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (960)
#else
#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#endif


//#include <pugixml.hpp>
#include <iostream>

#include "PlayerAnim1.h"
#include "PlayerAnimTuple1.h"
#include "CharacterAnim1.h"

#include "slime_data.h"

#include "RockAnim1.h"

/*
#include "TilemapData1.h"
#include "TilemapData2.h"
#include "TilemapData3.h"
#include "TilemapDataCollider1.h"
*/

#include "tilemap/tilemap_data_1_1.h"
#include "tilemap/tilemap_data_1_2.h"
#include "tilemap/tilemap_data_1_3.h"
#include "tilemap/tilemap_data_1_collider.h"

#include "tilemap/tilemap_data_2_1.h"
#include "tilemap/tilemap_data_2_2.h"
#include "tilemap/tilemap_data_2_3.h"
#include "tilemap/tilemap_data_2_collider.h"

#include "tilemap/tilemap_data_3_1.h"
#include "tilemap/tilemap_data_3_2.h"
#include "tilemap/tilemap_data_3_3.h"
#include "tilemap/tilemap_data_3_collider.h"


#include "nav_mesh_rect1.h"
#include "nav_mesh_bake1.h"



int CAT_SDLEngine::InitEngine()
{
    /*NavMeshSystem2D nmsys(nav_mesh_rect1(), Eigen::Vector2i(0, 0));
    Eigen::Vector2i x(200, 50);
    debug::debugLog("ID : %d\n",nmsys.get_id(&x));
    */


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        debug::debugLog("Error: Failed to initialize SDL!\n");
        return -1;
    }

    Uint32 flags = 0;
   
/*
#ifdef _DEBUG
    flags = 0;
#else
    flags = SDL_WINDOW_FULLSCREEN;
#endif
*/

    m_window = SDL_CreateWindow(
        "SDL_Image_test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        flags);

    if (m_window == nullptr)
    {
        debug::debugLog("Error: Failed to initialize window!\n");
        return -1;
    }

    InitWindowPosition();

    m_renderer = SDL_CreateRenderer(m_window,
                                    -1,
                                    SDL_RENDERER_ACCELERATED);

    if (m_renderer == nullptr)
    {
        
        debug::debugLog("Error: Failed to initialize renderer!");
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return -1;
    }


    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    int num = 0;

    for (auto i = 0, joystick_count = SDL_NumJoysticks(); i < joystick_count; i++)
    {
        if (!SDL_IsGameController(i)) continue;

        num++;
        this->AddController(i);
    }

    if (num > 0) {
        this->type_controller = PAD_CONTROLLER;
    }
    else {
        this->type_controller = KEY_MOUSE;
    }



    CAT_ImageStorage::create();

    m_projecter = new ImageProjecter();

    m_collider_manager = new ColliderManager();

    m_nav_mesh_sys = new NavMeshSystem2D(nav_mesh_rect1(), nav_mesh_bake1(), Vector2i(-(32 * 5), -(32 * 12)));

    return 0;
}

int CAT_SDLEngine::InitObject()
{

    object::NavMeshEntity::ObjectInitializer navmeshInit = {};
    navmeshInit.position = { 0,0,0 };
    navmeshInit.rotation = { 0,0,0 };
    navmeshInit.scale = { 1,1,1 };
    navmeshInit.rect_list = m_nav_mesh_sys->get_all_rect();
    navmeshInit.renderer = m_renderer;
    navmeshInit.projecter = m_projecter;

    nav_mesh_entity = new object::NavMeshEntity(navmeshInit);


    object::TilemapObject::ObjectInitializer tilemapInit = {};
    tilemapInit.position = { - 160,- 160,0 };
    tilemapInit.rotation = { 0,0,0 };
    tilemapInit.scale = { 1,1,1 };
    tilemapInit.tilemap_file_path = "./resource/imgs/tilemap1.png";
    tilemapInit.tilemap_init = tilemap_data_1_1();
    tilemapInit.renderer = this->m_renderer;
    tilemapInit.image_layer = 0;
    tilemapInit.image_alpha = 255;
    tilemapInit.projecter = this->m_projecter;
    tilemapInit.collider_layer = 0;
    tilemapInit.collider_width = 32;
    tilemapInit.tilemap_collider_init = tilemap_data_1_collider();
    tilemapInit.collision_magnitude = -1;
    tilemapInit.collision = -1;
    tilemapInit.collider_manager = this->m_collider_manager;

    field = new object::TilemapObject(tilemapInit);

    tilemapInit.position = { -160 + (32 * 10),-160,0 };
    tilemapInit.tilemap_init = tilemap_data_3_1();
    tilemapInit.tilemap_collider_init = tilemap_data_3_collider();
    field3 = new object::TilemapObject(tilemapInit);


    tilemapInit.position = { -160, -160 -(32 * 7),0};

    tilemapInit.tilemap_file_path = "./resource/imgs/tilemap1.png";
    tilemapInit.tilemap_init = tilemap_data_2_1();
    tilemapInit.image_layer = 0;
    tilemapInit.image_alpha = 255;

    tilemapInit.tilemap_collider_init = tilemap_data_2_collider();
    field2 = new object::TilemapObject(tilemapInit);



    tilemapInit.position = { -160, -160 -64,0 };

    tilemapInit.tilemap_file_path = "./resource/imgs/tilemap1.png";
    tilemapInit.tilemap_init = tilemap_data_1_2();
    tilemapInit.image_layer = 3;
    tilemapInit.image_alpha = 255;
    tilemapInit.collider_layer = 63;
    roof = new object::TilemapObject(tilemapInit);

    tilemapInit.position = { -160 + (32 * 10), -160 - 64,0};
    tilemapInit.tilemap_init = tilemap_data_3_2();
    roof3 = new object::TilemapObject(tilemapInit);

    tilemapInit.position = { -160, -160 - 64 - (32 * 7),0};
    tilemapInit.tilemap_init = tilemap_data_2_2();
    roof2 = new object::TilemapObject(tilemapInit);


    
    tilemapInit.position = { -160, -160 - 64,0 };
    tilemapInit.tilemap_init = tilemap_data_1_3();
    tilemapInit.image_layer = 1;
    tilemapInit.collider_layer = 63;
    wall = new object::TilemapObject(tilemapInit);

    tilemapInit.position = { -160 + (32 * 10), -160 - 64,0};
    tilemapInit.tilemap_init = tilemap_data_3_3();
    wall3 = new object::TilemapObject(tilemapInit);

    tilemapInit.position = { -160, -160 - 64 - (32 * 7),0 };
    tilemapInit.tilemap_init = tilemap_data_2_3();
    wall2 = new object::TilemapObject(tilemapInit);



    //gb1 = new GameObject("ObjectA", Vector3d(320, 240, 0), m_renderer, m_projecter, m_collider_manager);
    //gb1->set_scale(Vector3d(1.0, 1.0, 1.0));
    //gb1->set_input(&(this->input));

    object::AnimationEntity2D::ObjectInitializer gb2Init = {};
    gb2Init.position = Eigen::Vector3d(60, 0, 0);
    gb2Init.rotation = Eigen::Vector3d(0, 0, 0);
    gb2Init.scale = Eigen::Vector3d(1, 1, 1);
    gb2Init.animation_data = rock_anim_1();
    gb2Init.renderer = m_renderer;
    gb2Init.image_layer = 2;
    gb2Init.projecter = m_projecter;
    gb2Init.animation_sets = { {0,0,0},{0,1,0},{0,2,0},{0,3,0} };
    gb2Init.physics_type = component::CAT_Rigidbody::Newton;
    gb2Init.mass = 8.0;
    gb2Init.input_speed = M_PI * 2;
    gb2Init.max_speed = 150;
    gb2Init.collider_layer = 0;
    gb2Init.collider_w = 40;
    gb2Init.collider_h = 40;
    gb2Init.collider_offset = Vector2d(0, 0);
    gb2Init.collider_manager = m_collider_manager;
    gb2 = new object::AnimationEntity2D(gb2Init);

    object::Player2D::ObjectInitializer gb1Init = {};
    gb1Init.position = { 0, 0,0};
    gb1Init.rotation = Eigen::Vector3d(0, 0, 0);
    gb1Init.scale = Eigen::Vector3d(1, 1, 1);
    gb1Init.animation_data = player_anim_1();
    //gb1Init.animation_data = slime_data();
    gb1Init.renderer = m_renderer;
    gb1Init.image_offset = Eigen::Vector2i(0,-16);
    gb1Init.image_layer = 2;
    gb1Init.projecter = m_projecter;
    gb1Init.animation_sets = player_anim_tuple1();
    gb1Init.physics_type = component::CAT_Rigidbody::Newton;
    gb1Init.mass = 4.0;
    gb1Init.input_speed = M_PI * 2;
    gb1Init.max_speed = 150;
    gb1Init.collider_layer = 0;
    gb1Init.collider_w = 32;
    gb1Init.collider_h = 32;
    gb1Init.collider_offset = Vector2d(0, 0);
    //gb1Init.collider_w = 16;
    //gb1Init.collider_h = 16;
    //gb1Init.collider_offset = Vector2d(0, 8);
    gb1Init.collider_manager = m_collider_manager;
    gb1Init.player_input = &(this->input);
    gb1 = new object::Player2D(gb1Init);

    object::CameraObject::ObjectInitializer cameraInit = {};
    cameraInit.position = gb1Init.position;
    cameraInit.rotation = gb1Init.rotation;
    cameraInit.scale = gb1Init.scale;
    cameraInit.projecter_ptr = m_projecter;
    cameraInit.view_port_center = Eigen::Vector3d(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
    camera = new object::CameraObject(cameraInit);
    camera->SetTarget(gb1->GetTransform());

    object::Slime2D::ObjectInitializer gb3Init = {};
    gb3Init.position = { 320, 0,0 };
    gb3Init.rotation = Eigen::Vector3d(0, 0, 0);
    gb3Init.scale = Eigen::Vector3d(2, 2, 1);
    gb3Init.animation_data = slime_data();
    gb3Init.renderer = m_renderer;
    gb3Init.image_layer = 2;
    gb3Init.projecter = m_projecter;
    gb3Init.animation_sets = player_anim_tuple1();
    gb3Init.physics_type = component::CAT_Rigidbody::Newton;
    gb3Init.mass = 4.0;
    gb3Init.input_speed = M_PI * 2;
    gb3Init.max_speed = 100;
    gb3Init.collider_layer = 0;
    gb3Init.collider_w = 16;
    gb3Init.collider_h = 16;
    gb3Init.collider_offset = Vector2d(0, 8);
    gb3Init.collider_manager = m_collider_manager;
    gb3Init.target_trans_ptr = gb1->GetTransform();
    gb3Init.nmsys_ptr = m_nav_mesh_sys;
    gb3 = new object::Slime2D(gb3Init);
    




    return 0;
}


int CAT_SDLEngine::Update()
{
    frameStart = SDL_GetTicks();

    /*debug::print("window_pos", Eigen::Vector3d(window_pos[0], window_pos[1], 0));*/

    

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = 1;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (this->type_controller == CAT_SDLEngine::KEY_MOUSE) {

                if (e.key.keysym.sym == SDLK_d)
                {
                    input.right = 1;
                }


                if (e.key.keysym.sym == SDLK_a)
                {
                    input.left = 1;
                }


                if (e.key.keysym.sym == SDLK_w)
                {
                    input.front = 1;
                }

                if (e.key.keysym.sym == SDLK_s)
                {
                    input.back = 1;
                }

                if (e.key.keysym.sym == SDLK_j) {
                    input.right_trigger = 1;
                }


                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    input.escape = 1;
                }

            }

        }
        else if (e.type == SDL_KEYUP)
        {
            if (this->type_controller == CAT_SDLEngine::KEY_MOUSE) {

                if (e.key.keysym.sym == SDLK_d)
                {
                    input.right = 0;
                }


                if (e.key.keysym.sym == SDLK_a)
                {
                    input.left = 0;
                }


                if (e.key.keysym.sym == SDLK_w)
                {
                    input.front = 0;
                }


                if (e.key.keysym.sym == SDLK_s)
                {
                    input.back = 0;
                }

                if (e.key.keysym.sym == SDLK_j) {
                    input.right_trigger = 0;
                }

                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    input.escape = 0;
                }

            }



        }
        else if (e.type == SDL_CONTROLLERDEVICEADDED) {
            AddController(e.cdevice.which);

            this->type_controller = PAD_CONTROLLER;
            if (m_controller == nullptr) quit = 1;
        }
        else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
            RemoveController();

            this->type_controller = KEY_MOUSE;
        }
    }

    if (this->type_controller == CAT_SDLEngine::PAD_CONTROLLER) {
        if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_DPAD_UP) > 0) {
            input.front = 1;
        }
        else {
            input.front = 0;
        }

        if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) > 0) {
            input.left = 1;
        }
        else {
            input.left = 0;
        }

        if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) > 0) {
            input.right = 1;
        }
        else {
            input.right = 0;
        }

        if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) > 0) {
            input.back = 1;
        }
        else {
            input.back = 0;
        }

        if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_START) > 0) {
            input.escape = 1;
        }
        else {
            input.escape = 0;
        }



        float value = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        if (!input.right_trigger_switch) {
            if (value > 10000) {
                printf("%f\n", value);

                input.right_trigger = 1;
                input.right_trigger_switch = true;
            }
        }
        else {
            printf("%f\n", value);
            input.right_trigger = 0;

            if (value < 10000) {
                input.right_trigger_switch = false;
            }
        }

    }



        

    gb1->Update();
    gb2->Update();

    camera->Update();

    gb3->Update();

    this->Draw();

    this->m_collider_manager->judge();

    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < 15)
    {
        SDL_Delay(15 - frameTime);
        frameTime = SDL_GetTicks() - frameStart;
    }
    else {
        /*if (CheckWindowPosition()) {
            frameTime = 15;
            InitWindowPosition();
        }*/
        frameTime = 15;
    }

    //preDeltaTime = frameTime;

    debug::debugLog("%d\n", preDeltaTime);

    gb1->Gain(frameTime);
    gb2->Gain(frameTime);

    gb3->Gain(frameTime);

        



    count++;
    

    return 0;
}

int CAT_SDLEngine::Draw() {

    SDL_RenderClear(m_renderer);

    this->m_projecter->project();

    SDL_RenderPresent(m_renderer);

    return 0;
}

int CAT_SDLEngine::Finish(){
    if(!quit){
		SDL_Delay(5000);
	}

    delete nav_mesh_entity;

    delete camera;

    delete field;
    delete field2;
    delete field3;

    delete roof;
    delete roof2;
    delete roof3;

    delete wall;
    delete wall2;
    delete wall3;

    delete gb1;
    delete gb2;

    delete gb3;

	CAT_ImageStorage::destroy();
    delete m_projecter;
    delete m_collider_manager;

    delete m_nav_mesh_sys;

    this->RemoveController();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();


    return 0;


}

int CAT_SDLEngine::Judge(){
    return /*(count > 5000) ||*/ (input.escape > 0) || (quit > 0);
}


void CAT_SDLEngine::AddController(int index) {
    if (m_controller != nullptr) return;
    m_controller = SDL_GameControllerOpen(index);
    debug::debugLog("Open Game Controller!\n");

    InitInput();

    if (m_controller != nullptr) return;
    debug::debugLog("Error : Failed to Open Game Controller!\n");
}

void CAT_SDLEngine::RemoveController() {
    if (m_controller == nullptr) return;

    SDL_GameControllerClose(m_controller);
    m_controller = nullptr;

    InitInput();
}

void CAT_SDLEngine::InitInput() {
    input.front = 0;
    input.left = 0;
    input.right = 0;
    input.back = 0;

    input.right_trigger = 0;

    input.escape = 0;
}

bool CAT_SDLEngine::CheckWindowPosition() {
    int x, y;
    SDL_GetWindowPosition(m_window, &(x), &(y));

    if (window_pos[0] == x && window_pos[1] == y) {
        return false;
    }
    else {
        return true;
    }
}

void CAT_SDLEngine::InitWindowPosition() {
    int x, y;
    SDL_GetWindowPosition(m_window, &(x), &(y));

    window_pos[0] = x;
    window_pos[1] = y;
}
