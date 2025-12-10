#pragma once 

#include <iostream>
#include <vector>
#include <map>

#include "Animation.h"

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

class Object
{
private:
    std::map<ObjectPossibleLocation, Offset> object_location_params;
    void create_object_boarder(StaticObject & _obj);

public:
    StaticObject create(TileAnim & anim, ObjectParams params);

    Object();
    ~Object();
};

