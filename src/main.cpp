#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <vector>

#define WORLD_SIZE 10

int window_w = 0, window_h = 0;

struct ColorPack
{
    Uint8 r, g, b, a;
};

std::vector<std::vector<ColorPack>> color_seed;

void print_error(std::string base_function, std::string inner_function, std::string comment)
{
    std::cout << "[E] " << base_function << " :: " << inner_function << " -> " << comment << '\n';
}

bool create_single_block(SDL_Renderer* renderer, SDL_FRect block, ColorPack colors)
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

std::vector<std::vector<ColorPack>> get_random_seed()
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

bool create_random_mesh(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  
    SDL_RenderClear(renderer);

    int w, h;
    if(!SDL_GetWindowSize(window, &w, &h))
    {
        print_error("create_random_mesh", "SDL_GetWindowSize", " .... ");
        return false;
    }

    int block_w = floor(w / (double)WORLD_SIZE), 
        block_h = floor(h / (double)WORLD_SIZE);

    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            SDL_FRect th_block = {x * block_w, y * block_h, block_w, block_h}; //x, y, w, h
                        
            if(!create_single_block(renderer, th_block, color_seed[x][y]))
                std::cout << "skip " <<  x << "," << y << '\n';
        }
    }
    
    return true;
}

bool is_windowSizeChanged(SDL_Window* window)
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

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("MiniWorldGame",
                                          200, 200,
                                          SDL_WINDOW_RESIZABLE);
    if (!window) 
    {
        std::cerr << "CreateWindow failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) 
    {
        std::cerr << "CreateRenderer failed: " << SDL_GetError() << "\n";
        return 1;
    }

    bool running = true;
    SDL_Event event;

    
    if(!SDL_GetWindowSize(window, &window_w, &window_h))
    {
        print_error("main", "SDL_GetWindowSize", " .... ");
        exit(EXIT_FAILURE);
    }

    color_seed = get_random_seed();

    while (running) 
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = false;
            }
        }
        
        if(is_windowSizeChanged(window))
        {
            std::cout << "color seed has been changed " << '\n';
            color_seed = get_random_seed();       
        }
        create_random_mesh(window, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
