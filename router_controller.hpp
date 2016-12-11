#pragma once

#ifndef ROUTERCONTROLLER_H
#define ROUTERCONTROLLER_H


#include <memory>
#include <thread>
#include <functional>
#include <boost/asio.hpp>
#include "router.hpp"



class RouterController
{
private:
    std::shared_ptr<boost::asio::io_service> io_service = 
        std::make_shared<boost::asio::io_service>();

    Router router;

    std::unique_ptr<std::thread> routerThread;

    

    void run();


public:
	RouterController(std::string hostname, unsigned short local_port);

	Router& get_router() { return router; }

    using write_t = std::function<void(std::string)>;    
    const write_t& write = std::bind(&Router::send_msg, &router, std::placeholders::_1); 

    void start();
    
    void stop();

    void connect(std::string, std::string);
};



#endif // ROUTERCONTROLLER_H
