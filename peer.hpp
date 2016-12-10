#pragma once

#ifndef PEER_H
#define PEER_H


#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>



class Peer
{
private:
    std::string nickname;

    std::string remote_port;

    std::shared_ptr<boost::asio::ip::tcp::socket> sock; 

    std::shared_ptr<boost::asio::streambuf> buf =
        std::make_shared<boost::asio::streambuf>();

    std::shared_ptr<std::istream> ist = 
        std::make_shared<std::iostream>(buf.get());


public:
    Peer(std::shared_ptr<boost::asio::ip::tcp::socket>);
    
    ~Peer();

    inline std::string get_remote_address() 
    {
        return sock->remote_endpoint().address().to_string() + ':' + remote_port;;
    }

    inline std::shared_ptr<boost::asio::ip::tcp::socket> get_sock()
    {
        return sock;
    }

    inline void listen() 
    {
        boost::asio::async_read_until(*sock, *buf, '\n',
            boost::bind(&Peer::read_handler, this, _1, _2));
    }


    void write(const std::string&);

    void read_handler(const boost::system::error_code&, std::size_t);

    void write_handler(const boost::system::error_code&, std::size_t);
};



#endif // PEER_H