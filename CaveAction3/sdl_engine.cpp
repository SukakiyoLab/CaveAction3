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






int CAT_SDLEngine::InitEngine()
{
    /*
    {
        FunctionMap functionMap;
        XMLData* xmlData = functionMap.use_char_func("slime_anim_data");
        printf("data->animation->frame->filename : %s\n", xmlData->nexts["animation"][0]->nexts["frame"][0]->nexts["filename"][0]->item.c_str());
    }
    */

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

    functionMap = new FunctionMap();

    //auto nav_mesh_rect = nav_mesh_rect1();
    m_nav_mesh_sys = new NavMeshSystem2D(functionMap->use_char_func("nav_mesh_rect1"), functionMap->use_value_func("nav_mesh_bake1"), Vector2i(-(32 * 5), -(32 * 12)));

    object_generator = new game_system_3::ObjectGenerator();

    game_system_1::ObjectManager::ObjectManagerInitializer omInit;
    omInit.input_ptr = &(this->input);
    omInit.projecter_ptr = this->m_projecter;
    omInit.coll_man_ptr = this->m_collider_manager;
    omInit.renderer_ptr = this->m_renderer;
    omInit.nav_mesh_system_ptr = this->m_nav_mesh_sys;
    omInit.func_map_ptr = this->functionMap;
    object_manager = new game_system_1::ObjectManager(object_generator, omInit);

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

    nav_mesh_entity = new object::NavMeshEntity(&navmeshInit);


    object_generator->save_generate_object(functionMap->use_char_func("room_object_data1"));
    object_generator->save_generate_object(functionMap->use_char_func("room_object_data2"));
    object_generator->save_generate_object(functionMap->use_char_func("room_object_data3"));

    object_generator->save_generate_object(functionMap->use_char_func("rock_object_data"));
    object_generator->save_generate_object(functionMap->use_char_func("player_object_data"));
    object_generator->save_generate_object(functionMap->use_char_func("slime_object_data"));
    object_generator->save_generate_object(functionMap->use_char_func("camera_object_debug_data"));

    object_generator->save_delete_object("rock"); /* Šâ‚Ìíœ */


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

    object_manager->generate_objects();
    object_manager->delete_objects();
    object_manager->reset_generator();


    object_manager->gain(frameTime);

    this->m_collider_manager->judge();

    object_manager->update();
    
    this->Draw();
    
    

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

    //debug::debugLog("%d\n", preDeltaTime);


    count++;
    

    return 0;
}

int CAT_SDLEngine::Draw() {

    SDL_RenderClear(m_renderer);

    this->m_projecter->project();

    SDL_RenderPresent(m_renderer);

    return 0;
}

int CAT_SDLEngine::Finish() {
    if (!quit) {
        SDL_Delay(5000);
    }

    delete nav_mesh_entity;

    CAT_ImageStorage::destroy();
    delete m_projecter;
    delete m_collider_manager;

    delete m_nav_mesh_sys;
    delete functionMap;

    delete object_manager;
    delete object_generator;

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
