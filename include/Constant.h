#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <SDL3/SDL.h>

#define WORLD_SIZE 50

struct ColorPack
{
    int r, g, b, a;
};

enum LocMapEnum
{
    LM_X                = 0,
    LM_Y                = 1, 
    LocMapEnum_count
};

class Constant 
{
private:

public:
    static Uint64 freq;
    
    Constant();
    ~Constant();
   
    static void print_error(std::string base_function, std::string inner_function, std::string comment);
};