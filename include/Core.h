#pragma once 

#include "Constant.h"
#include <SDL3/SDL.h>
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
    
    bool init();
    bool deinit();

    bool is_windowSizeChanged(SDL_Window* window);
    bool create_random_mesh(SDL_Window* window, SDL_Renderer* renderer);
    std::vector<std::vector<ColorPack>> get_random_seed();
    
    bool create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors);
    bool create_single_block(SDL_FRect block, Tile tile);
    
    bool load_Tile(const char* path, Tile & tile);
    bool load_tile_library();
    std::vector<std::vector<Tile>> get_random_texture();


    //better remove 
    void print_error(std::string base_function, std::string inner_function, std::string comment);


public:
    void window_handler();

    Core();
    ~Core();
};