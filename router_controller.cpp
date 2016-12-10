#include "router_controller.hpp"



RouterController::RouterController(std::string hostname, unsigned short local_port)
	: router(io_service, hostname, local_port)
{}


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


void RouterController::connect(std::string host, std::string port)
{
	router.connect(host+':'+port);
}