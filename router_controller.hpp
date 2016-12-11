#pragma once

#ifndef ROUTERCONTROLLER_H
#define ROUTERCONTROLLER_H


#include <memory>
#include <thread>
#include <functional>
#include <boost/asio.hpp>
#include "router.hpp"


namespace Orthrus {



class RouterController
{
public:
    using send_t = std::function<void(std::string)>;
    using error_handler_t = std::function<void(std::exception&)>;

private:
    std::shared_ptr<boost::asio::io_service> io_service 
        = std::make_shared<boost::asio::io_service>();

    Router router;

    std::unique_ptr<std::thread> routerThread;

    error_handler_t error_handler;
    
    void run();


public:
	RouterController(std::string hostname, unsigned short local_port);

	inline Router& get_router() { return router; }
   
    const send_t send
        = std::bind(&Router::send_msg, &router, std::placeholders::_1); 

    void set_error_handler(error_handler_t&& eh);

    void start();
    
    void stop();

    void connect(std::string, std::string);
};


}


#endif // ROUTERCONTROLLER_H
