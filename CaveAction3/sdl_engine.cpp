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

#include "RockAnim1.h"

#include "TilemapData1.h"
#include "TilemapData2.h"
#include "TilemapData3.h"
#include "TilemapDataCollider1.h"

int CAT_SDLEngine::InitEngine()
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        debug::debugLog("Error: Failed to initialize SDL!\n");
        return -1;
    }

    Uint32 flags;
#ifdef _DEBUG
    flags = 0;
#else
    flags = SDL_WINDOW_BORDERLESS;
#endif

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

    m_renderer = SDL_CreateRenderer(m_window,
                                    -1,
                                    SDL_RENDERER_SOFTWARE);

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

    return 0;
}

int CAT_SDLEngine::InitObject()
{
    object::TilemapObject::ObjectInitializer tilemapInit = {};
    tilemapInit.position = { SCREEN_WIDTH / 2 - 160,SCREEN_HEIGHT / 2 - 160,0 };
    tilemapInit.rotation = { 0,0,0 };
    tilemapInit.scale = { 1,1,1 };
    tilemapInit.tilemap_file_path = "./resource/imgs/tilemap1.png";
    tilemapInit.tilemap_init = tilemap_data1();
    tilemapInit.renderer = this->m_renderer;
    tilemapInit.image_layer = 0;
    tilemapInit.projecter = this->m_projecter;
    tilemapInit.collider_layer = 0;
    tilemapInit.collider_width = 32;
    tilemapInit.tilemap_collider_init = tilemap_data_collider1();
    tilemapInit.collision_magnitude = -1;
    tilemapInit.collision = -1;
    tilemapInit.collider_manager = this->m_collider_manager;

    field = new object::TilemapObject(tilemapInit);


    tilemapInit.position = { SCREEN_WIDTH / 2 - 160,SCREEN_HEIGHT / 2 - 160 - 64,0 };

    tilemapInit.tilemap_init = tilemap_data2();
    tilemapInit.image_layer = 3;
    tilemapInit.collider_layer = 1;
    roof = new object::TilemapObject(tilemapInit);

    

    tilemapInit.tilemap_init = tilemap_data3();
    tilemapInit.image_layer = 1;
    tilemapInit.collider_layer = 2;
    wall = new object::TilemapObject(tilemapInit);



    //gb1 = new GameObject("ObjectA", Vector3d(320, 240, 0), m_renderer, m_projecter, m_collider_manager);
    //gb1->set_scale(Vector3d(1.0, 1.0, 1.0));
    //gb1->set_input(&(this->input));

    object::AnimationEntity2D::ObjectInitializer gb2Init = {};
    gb2Init.position = Eigen::Vector3d(SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2, 0);
    gb2Init.rotation = Eigen::Vector3d(0, 0, 0);
    gb2Init.scale = Eigen::Vector3d(1, 1, 1);
    gb2Init.animation_data = rock_anim_1();
    gb2Init.renderer = m_renderer;
    gb2Init.image_layer = 2;
    gb2Init.projecter = m_projecter;
    gb2Init.animation_sets = { {0,0,0},{0,1,0},{0,2,0},{0,3,0} };
    gb2Init.physics_type = component::CAT_Rigidbody::Newton;
    gb2Init.mass = 8.0;
    gb2Init.input_speed = M_PI;
    gb2Init.max_speed = 150;
    gb2Init.collider_layer = 0;
    gb2Init.collider_w = 40;
    gb2Init.collider_h = 40;
    gb2Init.collider_offset = Vector2d(0, 0);
    gb2Init.collider_manager = m_collider_manager;
    gb2 = new object::AnimationEntity2D(gb2Init);

    object::Player2D::ObjectInitializer gb1Init = {};
    gb1Init.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0};
    gb1Init.rotation = Eigen::Vector3d(0, 0, 0);
    gb1Init.scale = Eigen::Vector3d(1, 1, 1);
    gb1Init.animation_data = player_anim_1();
    gb1Init.renderer = m_renderer;
    gb1Init.image_layer = 2;
    gb1Init.projecter = m_projecter;
    gb1Init.animation_sets = player_anim_tuple1();
    gb1Init.physics_type = component::CAT_Rigidbody::Newton;
    gb1Init.mass = 4.0;
    gb1Init.input_speed = M_PI;
    gb1Init.max_speed = 150;
    gb1Init.collider_layer = 0;
    gb1Init.collider_w = 32;
    gb1Init.collider_h = 64;
    gb1Init.collider_offset = Vector2d(0, 0);
    gb1Init.collider_manager = m_collider_manager;
    gb1Init.player_input = &(this->input);
    gb1 = new object::Player2D(gb1Init);


    return 0;
}


int CAT_SDLEngine::Update()
{
    frameStart = SDL_GetTicks();

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

    
    
    this->m_collider_manager->judge();

    frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < 15)
    {
        SDL_Delay(15 - frameTime);
        frameTime = SDL_GetTicks() - frameStart;
    }

    preDeltaTime = frameTime;

    gb1->Update();
    gb2->Update();



    gb1->Gain(frameTime);
    gb2->Gain(frameTime);

    debug::debugLog("%d\n", frameTime);

    this->Draw();

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

    delete field;
    delete roof;
    delete wall;

    delete gb1;
    delete gb2;

	CAT_ImageStorage::destroy();
    delete m_projecter;
    delete m_collider_manager;

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