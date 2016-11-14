#pragma once
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include "router.hpp"



class Server
{
private:
    std::shared_ptr<boost::asio::io_service> io_service = 
        std::make_shared<boost::asio::io_service>();

    Router acceptor = Router(io_service);

    std::unique_ptr<std::thread> thread; 


    void run(void);


public:
    void start(void);
    void stop(void);
};