#include "peer.hpp"



using namespace Orthrus;


Peer::Peer(std::shared_ptr<boost::asio::ip::tcp::socket> sock_) : sock(sock_)
{
    ist = std::make_shared<std::iostream>(buf.get());
    boost::asio::read_until(*sock, *buf, '\n');
    std::getline(*ist, nickname);
    boost::asio::read_until(*sock, *buf, '\n');
    std::getline(*ist, remote_port);
    std::cout << nickname  << " joined" << std::endl;
}


Peer::Peer(std::shared_ptr<boost::asio::ip::tcp::socket> sock_,
        error_handler_t& eh) 
    : Peer(sock_)
{
    error_handler = eh;
}


Peer::~Peer() try
{
    sock->close();
}
catch (std::exception& e) { error_handler(e); }


void Peer::write(const std::string& msg) try
{
    boost::asio::async_write(*sock, boost::asio::buffer(msg), 
        boost::bind(&Peer::write_handler, this, _1, _2));
}
catch (std::exception& e) { error_handler(e); }


void Peer::read_handler(const boost::system::error_code& ec,
    std::size_t bytes_transferred) try
{
    if (ec) {
        throw std::runtime_error('['+nickname+']'+ec.message());
    }

    std::string msg;
    std::getline(*ist, msg);
    boost::asio::async_read_until(*sock, *buf, '\n', 
        boost::bind(&Peer::read_handler, this, _1, _2));
    
    read_msg_cb(nickname, msg);
}
catch (std::exception& e) { error_handler(e); }


void Peer::write_handler(const boost::system::error_code& ec, 
    std::size_t bytes_transferred) try
{
    if (ec) {
        throw std::runtime_error('['+nickname+']'+ec.message());
    }
}
catch (std::exception& e) { error_handler(e); }


void Peer::set_error_handler(error_handler_t& eh) try
{ 
    error_handler = eh; 
}
catch (std::exception& e) { error_handler(e); }
