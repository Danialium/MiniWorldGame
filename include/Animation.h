#pragma once 

#include "Constant.h"
#include <SDL3/SDL.h>
#include <toml++/toml.hpp>

struct TileAnim
{
    std::vector<SDL_Texture*> anim;
    int anim_count;

    int width, height;
    
    int id;
    int duration;
    Uint64 lastUpdate;
};

class Animation
{
private:
    Uint8 freq, crntCounter;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    std::map<std::string, TileAnim> animation_library;
    std::vector<std::vector<TileAnim>> tile_anim_seed;
    
    bool load_animation(const char* path);
    bool create_single_block(SDL_FRect block, TileAnim & anim);
    bool parse_anim_config(std::string path, 
        std::map<std::string, TileAnim> & animation_library,
        std::map<std::string, TileAnim>::iterator & it);
    bool update_random_animation();

    Animation(SDL_Window* window, SDL_Renderer* renderer);
    ~Animation();
};