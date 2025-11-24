#pragma once 

#include "Constant.h"
#include <SDL3/SDL.h>
struct Tile
{
    SDL_Texture* texture;
    int width, height;
};

struct TileAnim
{
    std::vector<SDL_Texture*> anim;
    int width, height;
    int id;
    int duration;
    long long duration_id;
    int anim_count;
};

class Core
{
private: 
    int window_w, window_h;
    std::vector<std::vector<ColorPack>> color_seed;
    std::vector<std::vector<Tile>> tile_seed;
    std::vector<std::vector<TileAnim>> tile_anim_seed;

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::map<std::string, Tile> texture_library;
    std::map<std::string, TileAnim> animation_library;
    
    bool init();
    bool deinit();

    bool is_windowSizeChanged(SDL_Window* window);
    bool create_random_mesh(SDL_Window* window, SDL_Renderer* renderer);
    
    bool load_tile_library();
    
    //color
    std::vector<std::vector<ColorPack>> get_random_seed();
    bool create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors);
    
    // texture
    bool load_Tile(const char* path, Tile & tile);
    std::vector<std::vector<Tile>> get_random_texture();
    bool create_single_block(SDL_FRect block, Tile & tile);
    
    //animation
    bool load_animation(const char* path, TileAnim & anim, std::string key, int count, int duration);
    std::vector<std::vector<TileAnim>> get_random_animation();
    bool create_single_block(SDL_FRect block, TileAnim & anim);



    //better remove 
    void print_error(std::string base_function, std::string inner_function, std::string comment);


public:
    void window_handler();

    Core();
    ~Core();
};