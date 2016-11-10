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
	Peer(std::shared_ptr<boost::asio::ip::tcp::socket>);
	
	~Peer();

	std::shared_ptr<boost::asio::ip::tcp::socket> sock;
	std::string port;

	inline std::string get_remote_address(void) {
		return sock->remote_endpoint().address().to_string() + ':' + port;
	}

	inline void listen(void) 
	{
		boost::asio::async_read_until(*sock, *buf, '\n',
			boost::bind(&Peer::read_handler, this, _1, _2));
	}

	void read_handler(const boost::system::error_code&, std::size_t);

	void write_handler(const boost::system::error_code&, std::size_t);
};
