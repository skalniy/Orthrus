#include "server.hpp"
#include <memory>



void Server::start(void) 
{
	std::thread t(&Server::run, this);
	thread.reset(new std::thread(move(t)));
}


void Server::stop(void) 
{
	io_service->stop();
	thread->join();
}


void Server::run(void)
{	
	acceptor.start();
	io_service->run();
}
