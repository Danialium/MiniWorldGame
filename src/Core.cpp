#include "Core.h"

Core::Core()
{
    window = nullptr;
    renderer = nullptr;
    window_w = window_h = 0;

    freq = SDL_GetPerformanceFrequency();

    if(!init())
    {
        print_error("Core", "init", " Exiting");
        exit(EXIT_FAILURE); 
    }

    if(!is_windowSizeChanged(window))
    {
        print_error("Core", "is_windowSizeChanged", " Exiting");
        exit(EXIT_FAILURE);
    }

    if(!load_animation("D:/All Projects/cpp/1404/MiniWorldGame/assets/tile/grass"))
    {
        print_error("Core", "load_animation", " Exiting");
        exit(EXIT_FAILURE);
    }   

    if(!load_animation("D:/All Projects/cpp/1404/MiniWorldGame/assets/character/human"))
    {
        print_error("Core", "load_animation", " Exiting");
        exit(EXIT_FAILURE);
    } 
}

Core::~Core()
{
    window = nullptr;
    renderer = nullptr;
}

//////////////////////////////////////////

bool Core::init()
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

    return true;
}

void Core::window_handler()
{
    //color_seed = get_random_seed();
    // tile_seed = get_random_texture();
    tile_anim_seed = get_random_animation();
    SDL_Event event;
    character_x = character_y = 0;
    desireX = desireY = 0;
    MovementParams moveParams;
    moveParams.speed = 1;
    moveParams.dst_loc[LM_X] = 0;
    moveParams.dst_loc[LM_Y] = 0;

    movementIns = std::make_unique<Movement>(moveParams);
    while (1) 
    {
        while (SDL_PollEvent(&event))
        {
            on_event_callback(event);
        }
        
        if(is_windowSizeChanged(window))
        {
            std::cout << "color seed has been changed " << '\n';
            //color_seed = get_random_seed();   
            // tile_seed = get_random_texture();
            tile_anim_seed = get_random_animation();
        }

        movementIns->update_movement(desireX, desireY);

        create_random_mesh(window, renderer);
        update_character_location(movementIns->movementParamsIns.dst_loc[LM_X], movementIns->movementParamsIns.dst_loc[LM_Y], animation_library["human"]);
        SDL_RenderPresent(renderer);
    }
}

bool Core::deinit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return true;
}

void Core::on_event_callback(SDL_Event & event)
{
    if (event.type == SDL_EVENT_QUIT) 
    {
        deinit();
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

bool Core::create_random_mesh(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  
    SDL_RenderClear(renderer);

    int block_w = floor(window_w / (double)WORLD_SIZE), 
        block_h = floor(window_h / (double)WORLD_SIZE);

    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            SDL_FRect th_block = {x * block_w, y * block_h, block_w, block_h}; //x, y, w, h
                        
            // if(!create_single_block(renderer, th_block, color_seed[x][y]))
            //     std::cout << "skip " <<  x << "," << y << '\n';

            // if(!create_single_block(th_block, tile_seed[x][y]))
            //     std::cout << "skip " <<  x << "," << y << '\n';

            if(!create_single_block(th_block, tile_anim_seed[x][y]))
                std::cout << "skip " <<  x << "," << y << '\n';
        }
    }
    
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////          Color             //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool Core::create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors)
{
    if(0)//set error for wrong size 
        return false;

    if(!SDL_SetRenderDrawColor(renderer, colors.r, colors.g, colors.b, colors.a))
    {
        print_error("create_single_block", "SDL_SetRenderDrawColor", " .... ");
        return false;
    }

    if(!SDL_RenderFillRect(renderer, &block))
    {
        print_error("create_single_block", "SDL_RenderFillRect", " .... ");
        return false;
    }

    return true;
}

std::vector<std::vector<ColorPack>> Core::get_random_seed()
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

bool Core::create_single_block(SDL_FRect block, Tile & tile)
{
    if(0)//set error for wrong size 
        return false;

    if(!SDL_RenderTexture(renderer, tile.texture, NULL, &block))
    {
        print_error("create_single_block", "SDL_RenderTexture", SDL_GetError());
        return false;
    }

    return true;
}

bool Core::load_Tile(const char* path, Tile & tile)
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

std::vector<std::vector<Tile>> Core::get_random_texture()
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
///////////////////////////////        ANIMATION           //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool Core::create_single_block(SDL_FRect block, TileAnim & anim)
{
    if(0)//set error for wrong size 
        return false;

    if(!SDL_RenderTexture(renderer, anim.anim[anim.id], NULL, &block))
    {
        print_error("create_single_block", "SDL_RenderTexture", SDL_GetError());
        return false;
    }

    crntCounter = SDL_GetPerformanceCounter();
    if(((crntCounter - anim.lastUpdate) * 1000) / freq >= anim.duration) //ms
    {
        anim.lastUpdate = crntCounter;
        anim.id = (anim.id + 1) % anim.anim_count;
    }      

    return true;
}

bool Core::load_animation(const char* path)
{
    std::map<std::string, TileAnim>::iterator it;
    if(!parse_anim_config(path, animation_library, it))
        return false;

    for (int i = 0; i < it->second.anim_count; i++)
    {
        SDL_Surface *surface = NULL;
        std::string image_path = (std::string)path + (std::string)"/" + it->first + "_" + std::to_string(i) + ".bmp";

        surface = SDL_LoadBMP(image_path.c_str());
        if (!surface)
        {
            SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
            return false;
        }

        it->second.width = surface->w;
        it->second.height = surface->h;

        it->second.anim[i] = SDL_CreateTextureFromSurface(renderer, surface);
        if (!it->second.anim[i]) 
        {
            SDL_Log("Couldn't create static texture: %s", SDL_GetError());
            return false;
        }

        SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */
    }   
}

std::vector<std::vector<TileAnim>> Core::get_random_animation()
{
    std::vector<std::vector<TileAnim>> output_seed(WORLD_SIZE, std::vector<TileAnim>(WORLD_SIZE));
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            output_seed[x][y] = animation_library["grass"];
            output_seed[x][y].id = rand() % output_seed[x][y].anim_count;
        }
    }

    return output_seed;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////            ETC             //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void Core::print_error(std::string base_function, std::string inner_function, std::string comment)
{
    std::cout << "[E] " << base_function << " :: " << inner_function << " -> " << comment << '\n';
}

bool Core::parse_anim_config(std::string path, std::map<std::string,
     TileAnim> & animation_library,
      std::map<std::string, TileAnim>::iterator & it)
{   
    try
    {
        std::string file_name = path + '/' + "config.toml";
        auto toml_read = toml::parse_file(file_name);
        
        std::string key = toml_read["tile"]["key"].value_or("");
        TileAnim th_anim;

        th_anim.id = 0;
        th_anim.duration = toml_read["tile"]["duration"].value_or(100);
        th_anim.anim_count = toml_read["tile"]["count"].value_or(1);
        th_anim.anim.resize(th_anim.anim_count);
        th_anim.height = th_anim.width = -1;
        th_anim.lastUpdate = SDL_GetPerformanceCounter();

        auto [_it, inserted] = animation_library.insert({key, th_anim});
        if(!inserted)
        {
            std::cout << "can not add this tile." << path << '\n';
            return false;
        }

        it = _it;
    }
    catch(const toml::parse_error& e)
    {
        std::cerr << e.description() << '\n';
        return false;
    }
    
    return true;
}

bool Core::load_tile_library()
{
    auto config = toml::parse_file("config.toml");
    int w     = config["graphics"]["width"].value_or(1280);
    int h     = config["graphics"]["height"].value_or(720);
    bool full = config["graphics"]["fullscreen"].value_or(false);
    config.clear();
    return true;
}

bool Core::is_windowSizeChanged(SDL_Window* window)
{
    int th_w = 0, th_h = 0;
    if(!SDL_GetWindowSize(window, &th_w, &th_h))
    {
        print_error("main", "SDL_GetWindowSize", " .... ");
        exit(EXIT_FAILURE);
    }

    bool ret = false;
    if(window_w != th_w || window_h != th_h)
    {
        ret = true;
        window_w = th_w;
        window_h = th_h;
    }

    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////          Character         //////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool Core::update_character_location(int x, int y, TileAnim & character)
{
    if(0)//set error for wrong size 
        return false;

    int block_w = floor(window_w / (double)WORLD_SIZE), 
        block_h = floor(window_h / (double)WORLD_SIZE);

    SDL_FRect th_block = {x, y, character.width, character.height};

    if(!SDL_RenderTexture(renderer, character.anim[character.id], NULL, &th_block))
    {
        print_error("create_single_block", "SDL_RenderTexture", SDL_GetError());
        return false;
    }

    crntCounter = SDL_GetPerformanceCounter();
    if(((crntCounter - character.lastUpdate) * 1000) / freq >= character.duration) //ms
    {
        character.lastUpdate = crntCounter;
        character.id = (character.id + 1) % character.anim_count;
    }      

    return true;
}
