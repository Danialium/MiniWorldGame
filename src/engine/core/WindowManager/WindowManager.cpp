#include "WindowManager.h"

WindowManager::WindowManager()
{
    window = nullptr;
    renderer = nullptr;
    window_w = window_h = 0;

    if(!init())
    {
        Constant::print_error("WindowManager", "init", " Exiting");
        exit(EXIT_FAILURE); 
    }

    if(!updateWindowSize(window))
    {
        Constant::print_error("WindowManager", "updateWindowSize", " Exiting");
        exit(EXIT_FAILURE);
    }
}

WindowManager::~WindowManager()
{
    window = nullptr;
    renderer = nullptr;
}

//////////////////////////////////////////

bool WindowManager::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("MiniWorldGame",
                                          200, 200,
                                          SDL_WINDOW_RESIZABLE);
    if (!window) 
    {
        std::cerr << "CreateWindow failed: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) 
    {
        std::cerr << "CreateRenderer failed: " << SDL_GetError() << "\n";
        return false;
    }

    objectIns = std::make_unique<Object>();

    return true;
}

void WindowManager::window_handler()
{
    //color_seed = get_random_seed();
    // tile_seed = get_random_texture();
    SDL_Event event;
    character_x = character_y = 0;
    desireX = desireY = 0;
    MovementParams moveParams;
    moveParams.speed = 1;
    moveParams.dst_loc[LM_X] = 0;
    moveParams.dst_loc[LM_Y] = 0;

    movementIns = std::make_unique<Movement>(moveParams);

    animationIns = std::make_unique<Animation>(window, renderer);
    animationIns->update_random_animation();


    //////////////////////////
    int block_w = (int)floor(window_w / (double)WORLD_SIZE), 
        block_h = (int)floor(window_h / (double)WORLD_SIZE);

    ObjectParams obj_params;
    obj_params.block_width = block_w;
    obj_params.block_height = block_h;
    obj_params.max_area = WORLD_SIZE * WORLD_SIZE / 4;
    obj_params.min_area = WORLD_SIZE * WORLD_SIZE / 16;
    obj_params.x_center = (WORLD_SIZE / 2) * block_w;
    obj_params.y_center = (WORLD_SIZE / 2) * block_h;
    
    mountain_object = objectIns->create(animationIns->animation_library["mountain"], obj_params);

    //////////////////////////


    while (1) 
    {
        while (SDL_PollEvent(&event))
        {
            on_event_callback(event);
        }

        movementIns->update_movement(desireX, desireY);

        create_random_mesh(window, renderer);
        update_character_location(movementIns->movementParamsIns.dst_loc[LM_X], movementIns->movementParamsIns.dst_loc[LM_Y], animationIns->animation_library["human"]);
        SDL_RenderPresent(renderer);
    }
}

bool WindowManager::deinit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return true;
}

void WindowManager::on_event_callback(SDL_Event & event)
{
    if (event.type == SDL_EVENT_QUIT) 
    {
        deinit();
        exit(EXIT_SUCCESS);
        return;
    }
    // else if(event.type ==SDL_EVENT_KEY_DOWN)
    // {
    //     /*
    //         SDL_SCANCODE_RIGHT = 79,
    //         SDL_SCANCODE_LEFT = 80,
    //         SDL_SCANCODE_DOWN = 81,
    //         SDL_SCANCODE_UP = 82,
    //     */
    //     int next_x = character_x , next_y = character_y;
    //     std::cout << event.key.scancode << '\n';
    //     switch (event.key.scancode )
    //     {
    //     case SDL_SCANCODE_RIGHT:
    //         next_x ++;
    //         break;
    //     case SDL_SCANCODE_LEFT:
    //         next_x --;
    //         break;
    //     case SDL_SCANCODE_DOWN:
    //         next_y ++;
    //         break;
    //     case SDL_SCANCODE_UP:
    //         next_y --;
    //         break;
        
    //     default:
    //         break;
    //     }

    //     character_x = std::max(window_w, 0);
    //     character_x = std::min(window_w, next_x);

    //     character_y = std::max(window_h, 0);
    //     character_y = std::min(window_h, next_y);

    //     std::cout << "SDL_EVENT_KEY_DOWN" << '\n';
    // }
    // else if(event.type ==SDL_EVENT_KEY_UP)
    // {
    //     std::cout << event.key.scancode << '\n';
    //     std::cout << "SDL_EVENT_KEY_UP" << '\n';
    // }
    else if (event.type == SDL_EVENT_WINDOW_RESIZED)
    {
        std::cout << "color seed has been changed " << '\n';    
       
        //FIXME : remove this setion after full size activated
        updateWindowSize(window);    
        window_w = window_h = std::max(window_w, window_h);
        SDL_SetWindowSize(window, window_w, window_h);
        updateWindowSize(window);    

        //color_seed = get_random_seed();   
        // tile_seed = get_random_texture();
        animationIns->update_random_animation();
        
        int block_w = (int)floor(window_w / (double)WORLD_SIZE),
        block_h = (int)floor(window_h / (double)WORLD_SIZE);

        ObjectParams obj_params;
        obj_params.block_width = block_w;
        obj_params.block_height = block_h;
        obj_params.max_area = WORLD_SIZE * WORLD_SIZE / 4;
        obj_params.min_area = WORLD_SIZE * WORLD_SIZE / 16;
        obj_params.x_center = (WORLD_SIZE / 2) * block_w;
        obj_params.y_center = (WORLD_SIZE / 2) * block_h;

        mountain_object = objectIns->create(animationIns->animation_library["mountain"], obj_params);
    }
    else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        desireX = event.button.x;
        desireY = event.button.y;

        movementIns->createThread();

        std::cout << "SDL_EVENT_MOUSE_BUTTON_DOWN" << '\n';
    }
    //update_character_location()
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool WindowManager::create_random_mesh(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  
    SDL_RenderClear(renderer);

    int block_w = (int)floor(window_w / (double)WORLD_SIZE), 
        block_h = (int)floor(window_h / (double)WORLD_SIZE);

    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            if (mountain_object.location.size() > 0)
            {
                bool is_found = false;
                for (auto & loc : mountain_object.location)
                {
                    int loc_x = (int)(loc.x / block_w);
                    int loc_y = (int)(loc.y / block_h);
                    
                    if (x == loc_x && y == loc_y)
                    {
                        if(!animationIns->create_single_block(loc, mountain_object.anim))
                            std::cout << "skip object " <<  x << "," << y << '\n';
                        is_found = true;
                        break;
                    }
                }

                if (is_found)
                    continue;
            }
            
            
            SDL_FRect th_block = {(float)x * block_w, (float)y * block_h, (float)block_w, (float)block_h}; //x, y, w, h
                        
            // if(!create_single_block(renderer, th_block, color_seed[x][y]))
            //     std::cout << "skip " <<  x << "," << y << '\n';

            // if(!create_single_block(th_block, tile_seed[x][y]))
            //     std::cout << "skip " <<  x << "," << y << '\n';

            if(!animationIns->create_single_block(th_block, animationIns->tile_anim_seed[x][y]))
                std::cout << "skip " <<  x << "," << y << '\n';
        }
    }
    
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////          Color             //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool WindowManager::create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors)
{
    if(0)//set error for wrong size 
        return false;

    if(!SDL_SetRenderDrawColor(renderer, colors.r, colors.g, colors.b, colors.a))
    {
        Constant::print_error("create_single_block", "SDL_SetRenderDrawColor", " .... ");
        return false;
    }

    if(!SDL_RenderFillRect(renderer, &block))
    {
        Constant::print_error("create_single_block", "SDL_RenderFillRect", " .... ");
        return false;
    }

    return true;
}

std::vector<std::vector<ColorPack>> WindowManager::get_random_seed()
{
    std::vector<std::vector<ColorPack>> output_seed(WORLD_SIZE, std::vector<ColorPack>(WORLD_SIZE));
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            ColorPack th_block_color = {rand() % 255, rand() % 255, rand() % 255, rand() % 255}; //r, g, b, a
            output_seed[x][y] = th_block_color;
        }
    }

    return output_seed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////         Texture            //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool WindowManager::create_single_block(SDL_FRect block, Tile & tile)
{
    if(0)//set error for wrong size 
        return false;

    if(!SDL_RenderTexture(renderer, tile.texture, NULL, &block))
    {
        Constant::print_error("create_single_block", "SDL_RenderTexture", SDL_GetError());
        return false;
    }

    return true;
}

bool WindowManager::load_Tile(const char* path, Tile & tile)
{
    SDL_Surface *surface = NULL;
    surface = SDL_LoadBMP(path);
    if (!surface)
    {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return false;
    }

    tile.width = surface->w;
    tile.height = surface->h;

    tile.texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!tile.texture) 
    {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return false;
    }

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    return true;
}

std::vector<std::vector<Tile>> WindowManager::get_random_texture()
{
    std::vector<std::vector<Tile>> output_seed(WORLD_SIZE, std::vector<Tile>(WORLD_SIZE));
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            output_seed[x][y] = texture_library["grass"];
        }
    }

    return output_seed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////            ETC             //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool WindowManager::load_tile_library()
{
    auto config = toml::parse_file("config.toml");
    int w     = config["graphics"]["width"].value_or(1280);
    int h     = config["graphics"]["height"].value_or(720);
    bool full = config["graphics"]["fullscreen"].value_or(false);
    config.clear();
    return true;
}

bool WindowManager::updateWindowSize(SDL_Window* window)
{
    int th_w = 0, th_h = 0;
    if(!SDL_GetWindowSize(window, &th_w, &th_h))
    {
        Constant::print_error("main", "SDL_GetWindowSize", " .... ");
        return false;
    }

    window_w = th_w;
    window_h = th_h;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////          Character         //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool WindowManager::update_character_location(int x, int y, TileAnim & character)
{
    if(0)//set error for wrong size 
        return false;

    int block_w = floor(window_w / (double)WORLD_SIZE), 
        block_h = floor(window_h / (double)WORLD_SIZE);

    SDL_FRect th_block = {x, y, block_w, block_h};

    if(!SDL_RenderTexture(renderer, character.anim[character.id], NULL, &th_block))
    {
        Constant::print_error("create_single_block", "SDL_RenderTexture", SDL_GetError());
        return false;
    }

    Uint8 crntCounter = SDL_GetPerformanceCounter();
    if(((crntCounter - character.lastUpdate) * 1000) / Constant::freq >= character.duration) //ms
    {
        character.lastUpdate = crntCounter;
        character.id = (character.id + 1) % character.anim_count;
    }      

    return true;
}
