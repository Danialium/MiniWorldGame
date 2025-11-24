#pragma once 

#include "Constant.h"
#include <SDL3/SDL.h>

class Core
{
private: 
    int window_w, window_h;
    std::vector<std::vector<ColorPack>> color_seed;

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    bool init();
    bool deinit();

    bool is_windowSizeChanged(SDL_Window* window);
    bool create_random_mesh(SDL_Window* window, SDL_Renderer* renderer);
    std::vector<std::vector<ColorPack>> get_random_seed();
    bool create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors);
    
    //better remove 
    void print_error(std::string base_function, std::string inner_function, std::string comment);


public:
    void window_handler();

    Core();
    ~Core();
};