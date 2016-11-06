#pragma once
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>



class Peer
{
private:
	std::shared_ptr<boost::asio::streambuf> buf =
		std::make_shared<boost::asio::streambuf>();

	std::shared_ptr<std::istream> ist;

	std::string name;


public:
	Peer(std::shared_ptr<boost::asio::ip::tcp::socket> sock_);
	
	~Peer();

	std::shared_ptr<boost::asio::ip::tcp::socket> sock;
	std::string port;

	void listen(void);

	void read_handler(const boost::system::error_code& ec,
		std::size_t bytes_transferred);

	void write_handler(const boost::system::error_code& error, 
  		std::size_t bytes_transferred);
};
