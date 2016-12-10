#include "router_controller.hpp"
#include <memory>



void RouterController::start(void) 
{
    routerThread.reset(new std::thread(&RouterController::run, this));
}


void RouterController::stop(void) 
{
    io_service->stop();
    routerThread->join();
}


void RouterController::run(void)
{   
    router.start();
    io_service->run();
}
