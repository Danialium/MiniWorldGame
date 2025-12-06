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

    bool is_static_tile;
};

struct ObjectParams
{
    int max_area;
    int min_area;
    int x_center;
    int y_center;

    int block_width;
    int block_height;
};

enum ObjectPossibleLocation
{
    OPL_CENTER = 0,
    OPL_TOP_LEFT = 1,
    OPL_TOP = 2,
    OPL_TOP_RIGHT = 3,
    OPL_RIGHT = 4,
    OPL_BOTTOM_RIGHT = 5,
    OPL_BOTTOM = 6,
    OPL_BOTTOM_LEFT = 7,
    OPL_LEFT = 8,
    ObjectPossibleLocation_count
};

struct StaticObject 
{
    TileAnim anim;
    std::vector<SDL_FRect> location;
};

struct Offset { int dx; int dy; };
struct Location { int x; int y; };

class Animation
{
private:
    Uint8 freq, crntCounter;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::map<ObjectPossibleLocation, Offset> object_location_params = {
        { OPL_CENTER,       { 0,  0} },
        { OPL_TOP_LEFT,     {-1, -1} },
        { OPL_TOP,          { 0, -1} },
        { OPL_TOP_RIGHT,    { 1, -1} },
        { OPL_RIGHT,        { 1,  0} },
        { OPL_BOTTOM_RIGHT, { 1,  1} },
        { OPL_BOTTOM,       { 0,  1} },
        { OPL_BOTTOM_LEFT,  {-1,  1} },
        { OPL_LEFT,         {-1,  0} }
    };

public:
    std::map<std::string, TileAnim> animation_library;
    std::vector<std::vector<TileAnim>> tile_anim_seed;
    
    bool load_animation(const char* path);
    bool create_single_block(SDL_FRect block, TileAnim & anim);
    bool parse_anim_config(std::string path, 
        std::map<std::string, TileAnim> & animation_library,
        std::map<std::string, TileAnim>::iterator & it);
    bool update_random_animation();

    StaticObject create_object(TileAnim & anim, ObjectParams params);

    Animation(SDL_Window* window, SDL_Renderer* renderer);
    ~Animation();
};