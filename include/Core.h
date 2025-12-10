#pragma once 

#include "Constant.h"
#include <SDL3/SDL.h>
#include <toml++/toml.hpp>
#include "Movement.h"
#include "Animation.h"
#include "Object.h"
#include <memory>

struct Tile
{
    SDL_Texture* texture;
    int width, height;
};

class Core
{
private: 
    int window_w, window_h;
    std::vector<std::vector<ColorPack>> color_seed;
    std::vector<std::vector<Tile>> tile_seed;

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::map<std::string, Tile> texture_library;

    std::unique_ptr<Animation> animationIns;
    
    //delete
    StaticObject mountain_object;
    
    std::unique_ptr<Object> objectIns;
    
    bool init();
    bool deinit();
    void on_event_callback(SDL_Event & event);

    bool updateWindowSize(SDL_Window* window);
    bool create_random_mesh(SDL_Window* window, SDL_Renderer* renderer);
    
    bool load_tile_library();
    
    //color
    std::vector<std::vector<ColorPack>> get_random_seed();
    bool create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors);
    
    // texture
    bool load_Tile(const char* path, Tile & tile);
    std::vector<std::vector<Tile>> get_random_texture();
    bool create_single_block(SDL_FRect block, Tile & tile);
    
    //
    bool update_character_location(int x, int y, TileAnim & character);
    int character_x, character_y;//TODO : temp
    int desireX, desireY;//TODO : temp

public:
    void window_handler();
    std::unique_ptr<Movement> movementIns;

    Core();
    ~Core();
};