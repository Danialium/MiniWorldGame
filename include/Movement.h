#pragma once 

#include "Constant.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

struct MovementParams
{
   int speed;  
   int dst_loc[LocMapEnum_count];
};

class Movement
{
private:
    int call_counter;
    //////////////////////////////

    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;
    bool is_moved;


    /////////////////////////////
    void main_thread();
    void destroy();
    void update_location();
    
public:
    MovementParams movementParamsIns;
    bool update_movement(int x, int y);
    bool createThread();
    int desire_locX, desire_locY;
    
    Movement(MovementParams _movementParamsIns);
    ~Movement();
};
