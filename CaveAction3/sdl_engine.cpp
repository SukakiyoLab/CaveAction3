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
    flags |= SDL_WINDOW_RESIZABLE;
   
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
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

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
    navmeshInit.projecter_ptr = m_projecter;
    navmeshInit.renderer_ptr = m_renderer;
    navmeshInit.transformInit.position = { 0,0,0 };
    navmeshInit.transformInit.rotation = { 0,0,0 };
    navmeshInit.transformInit.scale = { 1,1,1 };
    navmeshInit.rect_list = m_nav_mesh_sys->get_all_rect();
    navmeshInit.renderer = m_renderer;
    navmeshInit.projecter = m_projecter;

    nav_mesh_entity = new object::NavMeshEntity(navmeshInit);

    
    object::GameObject::ObjectInitializer objInit_tilemap = {};
    objInit_tilemap.projecter_ptr = m_projecter;
    objInit_tilemap.collider_manager_ptr = m_collider_manager;
    objInit_tilemap.renderer_ptr = m_renderer;
    objInit_tilemap.transformInit.position = { -160, -160, 0 };
    objInit_tilemap.transformInit.rotation = { 0, 0, 0 };
    objInit_tilemap.transformInit.scale = { 1, 1, 1 };
    component::CAT_Rigidbody::ComponentInitializer rigidbodyInit_tilemap = {};
    component::CAT_Tilemap::ComponentInitializer tilemapInit_tilemap = {};
    tilemapInit_tilemap.tilemap_path = "./resource/imgs/tilemap1.png";
    tilemapInit_tilemap.tilemap_init = tilemap_data_1_1();
    tilemapInit_tilemap.image_layer = 0;
    tilemapInit_tilemap.image_alpha = 255;
    component::CAT_TileCollider2D::ComponentInitializer tileCollider2DInit_tilemap = {};
    tileCollider2DInit_tilemap.layer = 0;
    tileCollider2DInit_tilemap.width = 32;
    tileCollider2DInit_tilemap.tilemap_collider_init = tilemap_data_1_collider();
    tileCollider2DInit_tilemap.magnitude = -1;
    tileCollider2DInit_tilemap.collision = -1;
    objInit_tilemap.selfComponentInits.push_back(&rigidbodyInit_tilemap);
    objInit_tilemap.selfComponentInits.push_back(&tilemapInit_tilemap);
    objInit_tilemap.selfComponentInits.push_back(&tileCollider2DInit_tilemap);
    field = new object::GameObject(objInit_tilemap);


    /*
    tilemapObjInit.transformInit.position = { -160 + (32 * 10),-160,0 };
    tilemapObjInit.tilemapInit.tilemap_init = tilemap_data_3_1();
    tilemapObjInit.tileColliderInit.tilemap_collider_init = tilemap_data_3_collider();
    field3 = new object::TilemapObject(tilemapObjInit);
    */

    objInit_tilemap.transformInit.position = { -160 + (32 * 10), - 160, 0 };
    tilemapInit_tilemap.tilemap_init = tilemap_data_3_1();
    tileCollider2DInit_tilemap.tilemap_collider_init = tilemap_data_3_collider();
    field3 = new object::GameObject(objInit_tilemap);


    objInit_tilemap.transformInit.position = { -160, -160 - (32 * 7),0 };
    tilemapInit_tilemap.tilemap_path = "./resource/imgs/tilemap1.png";
    tilemapInit_tilemap.tilemap_init = tilemap_data_2_1();
    tilemapInit_tilemap.image_layer = 0;
    tilemapInit_tilemap.image_alpha = 255;
    tileCollider2DInit_tilemap.tilemap_collider_init = tilemap_data_2_collider();
    field2 = new object::GameObject(objInit_tilemap);


    
    objInit_tilemap.transformInit.position = { -160, -160 - 64,0 };
    tilemapInit_tilemap.tilemap_path = "./resource/imgs/tilemap1.png";
    tilemapInit_tilemap.tilemap_init = tilemap_data_1_2();
    tilemapInit_tilemap.image_layer = 3;
    tilemapInit_tilemap.image_alpha = 255;
    tileCollider2DInit_tilemap.layer = 63;
    roof = new object::GameObject(objInit_tilemap);


    objInit_tilemap.transformInit.position = { -160 + (32 * 10), -160 - 64,0 };
    tilemapInit_tilemap.tilemap_init = tilemap_data_3_2();
    roof3 = new object::GameObject(objInit_tilemap);


    objInit_tilemap.transformInit.position = { -160, -160 - 64 - (32 * 7),0 };
    tilemapInit_tilemap.tilemap_init = tilemap_data_2_2();
    roof2 = new object::GameObject(objInit_tilemap);


    objInit_tilemap.transformInit.position = { -160, -160 - 64,0 };
    tilemapInit_tilemap.tilemap_init = tilemap_data_1_3();
    tilemapInit_tilemap.image_layer = 1;
    tileCollider2DInit_tilemap.layer = 63;
    wall = new object::GameObject(objInit_tilemap);



    objInit_tilemap.transformInit.position = { -160 + (32 * 10), -160 - 64,0 };
    tilemapInit_tilemap.tilemap_init = tilemap_data_3_3();
    wall3 = new object::GameObject(objInit_tilemap);



    objInit_tilemap.transformInit.position = { -160, -160 - 64 - (32 * 7),0 };
    tilemapInit_tilemap.tilemap_init = tilemap_data_2_3();
    wall2 = new object::GameObject(objInit_tilemap);




    object::GameObject::ObjectInitializer rockInit = {};
    rockInit.projecter_ptr = m_projecter;
    rockInit.collider_manager_ptr = m_collider_manager;
    rockInit.renderer_ptr = m_renderer;
    rockInit.transformInit.position = { 60,0,0 };
    rockInit.transformInit.rotation = { 0,0,0 };
    rockInit.transformInit.scale = { 1,1,1 };
    component::CAT_AnimationImage::ComponentInitializer animImageInit_rock = {};
    animImageInit_rock.xml_file = rock_anim_1();
    animImageInit_rock.image_layer = 2;
    component::CAT_Animator2D::ComponentInitializer animator2DInit_rock = {};
    animator2DInit_rock.animation_sets = { {0,0,0},{0,1,0},{0,2,0},{0,3,0} };
    component::CAT_Rigidbody::ComponentInitializer rigidbodyInit_rock = {};
    rigidbodyInit_rock.type = component::CAT_Rigidbody::Newton;
    rigidbodyInit_rock.mass = 8.0;
    component::CAT_VirtualController::ComponentInitializer virtualControllerInit_rock = {};
    virtualControllerInit_rock.input_speed = M_PI * 2;
    virtualControllerInit_rock.max_speed = 150;
    component::CAT_BoxCollider2D::ComponentInitializer boxCollider2DInit_rock = {};
    boxCollider2DInit_rock.layer = 0;
    boxCollider2DInit_rock.w = 40;
    boxCollider2DInit_rock.h = 40;
    boxCollider2DInit_rock.offset = { 0,0 };
    rockInit.selfComponentInits.push_back(&animImageInit_rock);
    rockInit.selfComponentInits.push_back(&animator2DInit_rock);
    rockInit.selfComponentInits.push_back(&rigidbodyInit_rock);
    rockInit.selfComponentInits.push_back(&virtualControllerInit_rock);
    rockInit.selfComponentInits.push_back(&boxCollider2DInit_rock);
    gb2 = new object::GameObject(rockInit);



    object::GameObject::ObjectInitializer plObjInit = {};
    plObjInit.projecter_ptr = m_projecter;
    plObjInit.collider_manager_ptr = m_collider_manager;
    plObjInit.renderer_ptr = m_renderer;
    plObjInit.transformInit.position = { 0,0,0 };
    plObjInit.transformInit.rotation = { 0,0,0 };
    plObjInit.transformInit.scale = { 1,1,1 };
    component::CAT_AnimationImage::ComponentInitializer animImageInit;
    animImageInit.xml_file = player_anim_1();
    animImageInit.offset = Eigen::Vector2i(0, -16);
    animImageInit.image_layer = 2;
    component::CAT_Animator2D::ComponentInitializer animatorInit;
    animatorInit.animation_sets = player_anim_tuple1();
    component::CAT_Rigidbody::ComponentInitializer rigidbodyInit;
    rigidbodyInit.type = component::CAT_Rigidbody::Newton;
    rigidbodyInit.mass = 4.0;
    component::CAT_VirtualController::ComponentInitializer virtualControllerInit;
    virtualControllerInit.input_speed = M_PI * 2;
    virtualControllerInit.max_speed = 150;
    component::CAT_BoxCollider2D::ComponentInitializer boxColliderInit;
    boxColliderInit.layer = 0;
    boxColliderInit.w = 32;
    boxColliderInit.h = 32;
    boxColliderInit.offset = { 0.0,0.0 };
    component::CAT_PlayerController::ComponentInitializer playerControllerInit;
    playerControllerInit.player_input_ptr = &(this->input);
    plObjInit.selfComponentInits.push_back(&animImageInit);
    plObjInit.selfComponentInits.push_back(&animatorInit);
    plObjInit.selfComponentInits.push_back(&rigidbodyInit);
    plObjInit.selfComponentInits.push_back(&virtualControllerInit);
    plObjInit.selfComponentInits.push_back(&boxColliderInit);
    plObjInit.selfComponentInits.push_back(&playerControllerInit);
    playerObj = new object::GameObject(plObjInit);


    object::GameObject::ObjectInitializer cameraInit = {};
    cameraInit.projecter_ptr = m_projecter;
    cameraInit.transformInit.position = plObjInit.transformInit.position;
    cameraInit.transformInit.rotation = plObjInit.transformInit.rotation;
    cameraInit.transformInit.scale = plObjInit.transformInit.scale;
    component::CAT_ViewCamera::ComponentInitializer viewCameraInit;
    viewCameraInit.view_port_center = Eigen::Vector3d(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
    viewCameraInit.other_transform_name = "player::CAT_Transform";
    viewCameraInit.other_transform_id = 0;
    cameraInit.selfComponentInits.push_back(&viewCameraInit);
    cameraInit.other_obj_components["player::CAT_Transform"].push_back(playerObj->GetTransform());
    camera = new object::GameObject(cameraInit);


    object::GameObject::ObjectInitializer slmObjInit;
    slmObjInit.projecter_ptr = m_projecter;
    slmObjInit.collider_manager_ptr = m_collider_manager;
    slmObjInit.nav_mesh_system_ptr = m_nav_mesh_sys;
    slmObjInit.renderer_ptr = m_renderer;
    slmObjInit.transformInit.position = { 320,0,0 };
    slmObjInit.transformInit.rotation = { 0,0,0 };
    slmObjInit.transformInit.scale = { 2,2,1 };
    slmObjInit.other_obj_components["player::CAT_Transform"].push_back(playerObj->GetTransform());
    animImageInit.xml_file = slime_data();
    animImageInit.image_layer = 2;
    animatorInit.animation_sets = player_anim_tuple1();
    rigidbodyInit.type = component::CAT_Rigidbody::Newton;
    rigidbodyInit.mass = 4.0;
    virtualControllerInit.input_speed = M_PI * 2;
    virtualControllerInit.max_speed = 100;
    boxColliderInit.layer = 0;
    boxColliderInit.w = 16;
    boxColliderInit.h = 16;
    boxColliderInit.offset = { 0.0,0.0 };
    component::CAT_NavMeshAgent2D::ComponentInitializer navMeshAgent2DInit;
    navMeshAgent2DInit.target_transform_name = "player::CAT_Transform";
    navMeshAgent2DInit.target_transform_id = 0;
    component::CAT_SlimeController::ComponentInitializer slimeControllerInit;
    slmObjInit.selfComponentInits.push_back(&animImageInit);
    slmObjInit.selfComponentInits.push_back(&animatorInit);
    slmObjInit.selfComponentInits.push_back(&rigidbodyInit);
    slmObjInit.selfComponentInits.push_back(&virtualControllerInit);
    slmObjInit.selfComponentInits.push_back(&boxColliderInit);
    slmObjInit.selfComponentInits.push_back(&navMeshAgent2DInit);
    slmObjInit.selfComponentInits.push_back(&slimeControllerInit);
    slimeObj = new object::GameObject(slmObjInit);
    

    





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



        

    //gb1->Update();
    gb2->Update();

    camera->Update();

    //gb3->Update();

    playerObj->Update();

    slimeObj->Update();

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

    //gb1->Gain(frameTime);
    gb2->Gain(frameTime);

    //gb3->Gain(frameTime);

    playerObj->Gain(frameTime);
    slimeObj->Gain(frameTime);



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

    //delete gb1;
    delete gb2;

    delete playerObj;
    delete slimeObj;

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
