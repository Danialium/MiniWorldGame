#include "Core.h"
#include <thread>

int main(int argc, char** argv)
{
    Core core_window;
    core_window.window_handler();
    //std::thread th_thread1(&Core::window_handler, &core_window);
    //std::thread th_thread2(&Movement::createThread, core_window.movementIns.get());
    
    //th_thread1.join();

    std::cout << "Done !" << '\n';

    return 0;
}
