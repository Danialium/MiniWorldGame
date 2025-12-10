#include "Object.h"

Object::Object()
{
    object_location_params = {
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
}

Object::~Object()
{
    
}

StaticObject Object::create(TileAnim & anim, ObjectParams params)
{
    StaticObject th_obj;
    bool is_object_done = false;
   
    int th_area = (rand() % (params.max_area - params.min_area)) + params.min_area;
    
    int x_pos = params.x_center;
    int y_pos = params.y_center;
    int area_count = 0;
    std::vector<Location> queue_locations;
    queue_locations.push_back({x_pos / params.block_width , y_pos / params.block_height});

    while (!is_object_done)
    {        
        SDL_FRect block = { 
            (float)queue_locations[area_count].x * params.block_width,
            (float)queue_locations[area_count].y * params.block_height,
            (float)params.block_width,
            (float)params.block_height 
        };
        
        th_obj.location.push_back(block);
        th_obj.anim = anim;
        
        for (int i = 0; i < ObjectPossibleLocation_count; i++)
        {
            int th_x = queue_locations[area_count].x + object_location_params[static_cast<ObjectPossibleLocation>(i)].dx;
            int th_y = queue_locations[area_count].y + object_location_params[static_cast<ObjectPossibleLocation>(i)].dy;

            auto it = std::find_if(queue_locations.begin(), queue_locations.end(), [&](const Location& p) {
                    return p.x == th_x && p.y == th_y;
            });

            if(it == queue_locations.end() && rand() % 100 > 30) //70% chance to add
            {
                queue_locations.push_back({
                    th_x, th_y
                });
            }
        }

        area_count++;

        if(area_count >= th_area)
            is_object_done = true;
    }
    
    return th_obj;
}

void Object::create_object_boarder(StaticObject & _obj)
{

}
