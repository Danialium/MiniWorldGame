#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <map>

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

class CConstant 
{
public:
    CConstant();
    ~CConstant();
    
private:

};