#pragma once

#ifndef ROUTER_H
#define ROUTER_H


#include <memory>
#include <thread>
#include <unordered_set>
#include <map>
#include <boost/asio.hpp>
#include "peer.hpp"



namespace Orthrus {


class Router
{
public:
    using send_t = std::function<void(std::string)>;
    using error_handler_t = std::function<void(std::exception&)>;

private:
    std::string hostname;

    boost::asio::ip::tcp::endpoint ep;

    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;

    std::map<std::string, std::shared_ptr<Peer>> peers;

    error_handler_t error_handler = 0;

    void init();

    void accept_handler(const boost::system::error_code& error,
        std::shared_ptr<boost::asio::ip::tcp::socket> sock);

    void share_peers(std::shared_ptr<boost::asio::ip::tcp::socket> target);

    void read_peers(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

    


public:
    Router(std::shared_ptr<boost::asio::io_service> io_service, 
        std::string hostname, unsigned short local_port);
    
    void start();

    void stop();

    void connect(std::string);

    void send_msg(std::string msg);

    void set_error_handler(error_handler_t& eh);
    
};


}



#endif // ROUTER_H
