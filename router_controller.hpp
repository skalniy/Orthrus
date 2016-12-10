#pragma once

#ifndef ROUTERCONTROLLER_H
#define ROUTERCONTROLLER_H


#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include "router.hpp"



class RouterController
{
private:
    std::shared_ptr<boost::asio::io_service> io_service = 
        std::make_shared<boost::asio::io_service>();

    Router router = Router(io_service);

    std::unique_ptr<std::thread> routerThread; 


    void run();


public:
    void start();
    
    void stop();
};



#endif // ROUTERCONTROLLER_H
