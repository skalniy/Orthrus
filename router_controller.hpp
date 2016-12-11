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
    using read_msg_cb_t = std::function<void(std::string, std::string)>;

private:
    std::shared_ptr<boost::asio::io_service> io_service 
        = std::make_shared<boost::asio::io_service>();

    error_handler_t error_handler;

    Router router;

    std::unique_ptr<std::thread> routerThread;
   
    void run();


public:
	RouterController(std::string hostname, unsigned short local_port);
    RouterController(std::string hostname, unsigned short local_port, error_handler_t&&);

	inline Router& get_router() { return router; }
   
    const send_t send
        = std::bind(&Router::send_msg, &router, std::placeholders::_1); 

    void set_error_handler(error_handler_t&& eh);

    void start();
    
    void stop();

    void connect(std::string, std::string);

    void set_read_msg_cb(read_msg_cb_t&& cb) { router.set_read_msg_cb(cb); }
};


}


#endif // ROUTERCONTROLLER_H
