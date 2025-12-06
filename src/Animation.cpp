#include "Animation.h"

Animation::Animation(SDL_Window* in_window, SDL_Renderer* in_renderer)
{
    window = in_window;
    renderer = in_renderer;

    //TODO : clean this section
    if(!load_animation("D:/All Projects/cpp/1404/MiniWorldGame/assets/tile/grass"))
    {
        Constant::print_error("Core", "load_animation", " Exiting");
        exit(EXIT_FAILURE);
    }   

    if(!load_animation("D:/All Projects/cpp/1404/MiniWorldGame/assets/character/human"))
    {
        Constant::print_error("Core", "load_animation", " Exiting");
        exit(EXIT_FAILURE);
    } 

}

Animation::~Animation()
{

}

bool Animation::create_single_block(SDL_FRect block, TileAnim & anim)
{
    if(0)//set error for wrong size 
        return false;

    if(!SDL_RenderTexture(renderer, anim.anim[anim.id], NULL, &block))
    {
        Constant::print_error("create_single_block", "SDL_RenderTexture", SDL_GetError());
        return false;
    }

    Uint8 crntCounter = SDL_GetPerformanceCounter();
    if(((crntCounter - anim.lastUpdate) * 1000) / freq >= anim.duration) //ms
    {
        anim.lastUpdate = crntCounter;
        anim.id = (anim.id + 1) % anim.anim_count;
    }      

    return true;
}

bool Animation::load_animation(const char* path)
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

bool Animation::update_random_animation()
{
    tile_anim_seed.clear();
    tile_anim_seed.resize(WORLD_SIZE, std::vector<TileAnim>(WORLD_SIZE));

    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            tile_anim_seed[x][y] = animation_library["grass"];
            tile_anim_seed[x][y].id = rand() % tile_anim_seed[x][y].anim_count;
        }
    }
    
    return true;
}

bool Animation::parse_anim_config(std::string path, 
    std::map<std::string, TileAnim> & animation_library,
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

