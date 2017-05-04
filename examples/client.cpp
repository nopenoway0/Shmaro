#include <boost/system/error_code.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <engine_info.h>

//#define _WIN32_WINNT                  0x0A00 // Windows 10

#define BUFFER_SIZE 5

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
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	if(std::string(argv[1]) == std::string("client")){
		std::string request;
		udp::endpoint main_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345);
		udp::socket connector(io_service);
		std::cout << "Starting Client..." <<std::endl;
		//udp::resolver::query query(udp::v4(), "127.0.0.1", "12345");
		connector.open(udp::v4());
		boost::array<Engine_Info, 5> buffer;

		//add Engine Info
		buffer[0] = Engine_Info(1,2.0);
		buffer[1] = Engine_Info(1,3.1);
		buffer[2] = Engine_Info(3,2.3);
		buffer[3] = Engine_Info(323, 4);
		//buffer[1].set_flag(ENGINE_END);
		//while(//strcmp(request.c_str(), "exit") != 0){
			//std::cin >> request;
			//std::copy(request.begin(), request.end(), buffer.data());
			//connector.send_to(boost::asio::buffer(buffer), main_endpoint);
		//}
		try{
			connector.send_to(boost::asio::buffer(buffer), main_endpoint);
		}catch(...){
			std::cout <<"error";
		}
		//sleep(8);
		for(int x = 0; x < BUFFER_SIZE; x++){
			buffer[x] = Engine_Info();
		}
		buffer[0] = Engine_Info(3,2.3,1);
		//buffer[0].set_flag(ENGINE_END);
		connector.send_to(boost::asio::buffer(buffer), main_endpoint);


		//std::getchar();
		connector.close();
	}
	else if(std::string(argv[1]) == std::string("server")){
		std::cout << "Starting Server" << std::endl;
		while(true){
			boost::array<char, 128> buffer = {{0}};
			udp::endpoint main_endpoint;
			udp::socket connector(io_service, udp::endpoint(udp::v4(), 12345));
			size_t len = connector.receive_from(boost::asio::buffer(buffer), main_endpoint);
			if(strcmp(buffer.data(), "exit") == 0) exit(1);
			//len = connector.receive(boost::asio::buffer(buffer));
			std::cout.write(buffer.data(), len);
			//sleep(2);
		}
	} 
	return 1;
}