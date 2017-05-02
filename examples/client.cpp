#include <boost/system/error_code.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

#define _WIN32_WINNT                  0x0A00 // Windows 10
namespace boost
{
#ifdef BOOST_NO_EXCEPTIONS
void throw_exception( std::exception const & e ){
    throw 11; // or whatever
};
#endif
}// namespace boost

using boost::asio::ip::udp;

int main(int argc, char* argv[]){
	if(sizeof(argv) == 0) return 0;
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::endpoint main_endpoint;
	if(std::string(argv[1]) == std::string("client")){
		udp::socket connector(io_service);
		std::cout << "Starting Client..." <<std::endl;
		udp::resolver::query query(udp::v4(), "127.0.0.1", "12345");
		main_endpoint = *resolver.resolve(query);
		connector.open(udp::v4());
		boost::array<char, 128> buffer = {0};
		buffer[0] = 'T';
		buffer[1] = 'E';
		buffer[2] = 'S';
		buffer[3] = 'T';
		connector.send_to(boost::asio::buffer(buffer), main_endpoint);
		std::cout << main_endpoint.address();
		std::getchar();
		connector.close();
	}
	else if(std::string(argv[1]) == std::string("server")){
		std::cout << "Starting Server" << std::endl;
		boost::array<char, 128> buffer = {{0}};
		udp::socket connector(io_service, udp::endpoint(udp::v4(), 12345));
		size_t len = connector.receive_from(boost::asio::buffer(buffer), main_endpoint);
		std::cout.write(buffer.data(), len);
		std::getchar();
		connector.close();
	} 
	return 1;
}