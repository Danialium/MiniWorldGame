#include "Movement.h"

Movement::Movement(MovementParams _movementParamsIns)
{
    movementParamsIns = _movementParamsIns;
    call_counter = 0;

    stop = false;
    is_moved = false;
}

Movement::~Movement()
{
    destroy();
}

void Movement::destroy()
{
    stop = true;
    cv.notify_all();
}

////////////////////////////////////////////////////////////////////////////////

bool Movement::createThread()
{
    std::thread th_thread(&Movement::main_thread, this);

    th_thread.detach();

    return true;
}

void Movement::main_thread()
{
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Movement thread started.\n";
    while (!stop) 
    {
        cv.wait(lock, [this] { return is_moved || stop; });

        if (stop) 
            break;
        
        lock.unlock();      
        ///////////////////////////
        
        update_location();
        
        ///////////////////////////
        is_moved = false;
        lock.lock();
    }
    std::cout << "Movement thread exiting.\n";
}

bool Movement::update_movement(int x, int y)
{
    std::lock_guard<std::mutex> lg(mtx);
    if (movementParamsIns.dst_loc[LM_X] == x &&
        movementParamsIns.dst_loc[LM_Y] == y)
    {
        stop = true;
    }
    else  
    {
        desire_locX = x;
        desire_locY = y;
        call_counter++;
        is_moved = true;
        stop = false;
    }  

    cv.notify_one();

    return true;
}

void Movement::update_location()
{
    if (call_counter%movementParamsIns.speed != 0)
    {
        return;
    }
    
    if (movementParamsIns.dst_loc[LM_X] < desire_locX)
        movementParamsIns.dst_loc[LM_X]++;
    else if (movementParamsIns.dst_loc[LM_X] > desire_locX)
        movementParamsIns.dst_loc[LM_X]--;

    if (movementParamsIns.dst_loc[LM_Y] < desire_locY)
        movementParamsIns.dst_loc[LM_Y]++;
    else if (movementParamsIns.dst_loc[LM_Y] > desire_locY)
        movementParamsIns.dst_loc[LM_Y]--;
    
}