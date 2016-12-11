#pragma once

#ifndef PEER_H
#define PEER_H


#include <memory>
#include <iostream>
#include <functional>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>



namespace Orthrus {


class Peer
{
public:
    using error_handler_t = std::function<void(std::exception&)>;
    using read_msg_cb_t = std::function<void(std::string, std::string)>;

    using disconnect_handler_t  = std::function<void()>;
    using connect_handler_t     = std::function<void()>;

private:
    error_handler_t error_handler;

    

    read_msg_cb_t read_msg_cb; 

    std::string nickname;

    std::string remote_port;

    std::shared_ptr<boost::asio::ip::tcp::socket> sock; 

    std::shared_ptr<boost::asio::streambuf> buf =
        std::make_shared<boost::asio::streambuf>();

    std::shared_ptr<std::istream> ist = 
        std::make_shared<std::iostream>(buf.get());



public:
    Peer(std::shared_ptr<boost::asio::ip::tcp::socket>);
    Peer(std::shared_ptr<boost::asio::ip::tcp::socket>, error_handler_t&);

    void set_read_msg_cb(read_msg_cb_t& cb) { read_msg_cb = cb; }

    disconnect_handler_t disconnect_handler;
    connect_handler_t connect_handler;

    std::string get_nickname() { return nickname; }
    
    ~Peer();

    inline std::string get_remote_address() 
    { return sock->remote_endpoint().address().to_string() 
        + ':' + remote_port; }

    inline std::shared_ptr<boost::asio::ip::tcp::socket> get_sock()
    { return sock; }

    inline void listen() 
    { 
        boost::asio::async_read_until(*sock, *buf, '\n',
            boost::bind(&Peer::read_handler, this, _1, _2));
        read_msg_cb(nickname, "CONNECTED");
    }

    void set_error_handler(error_handler_t& eh);

    void write(const std::string&);

    void read_handler(const boost::system::error_code&, std::size_t);

    void write_handler(const boost::system::error_code&, std::size_t);
};


}



#endif // PEER_H