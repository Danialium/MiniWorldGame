#include "Constant.h"

Uint64 Constant::freq = SDL_GetPerformanceFrequency();

Constant::Constant()
{

}

Constant::~Constant()
{

}

void Constant::print_error(std::string base_function, std::string inner_function, std::string comment)
{
    std::cout << "[E] " << base_function << " :: " << inner_function << " -> " << comment << '\n';
}
