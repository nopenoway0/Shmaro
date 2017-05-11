#ifndef GET_OBJ_H
#define GET_OBJ_H
#include <engine_info.h>

#include <boost/system/error_code.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

//#define _WIN32_WINNT                  0x0A00
namespace{
	#define BUFFER_SIZE 5
	using boost::asio::ip::udp;

	/*
	Engine_Info start_socket(){
		Engine_Info engine1(1, 2.0);
		return engine1;
	}*/
	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::socket connector(io_service, udp::endpoint(udp::v6(), 12345));
	boost::mutex _eng_access;
	std::vector<Engine_Info> info_list;
	bool server_done;

	// Synchronously reads from server
	// Will keep connection open until packets are recieved
	void start_server(){
		io_service.reset();
		server_done = false;
		size_t len = 0;
		boost::array<Engine_Info, BUFFER_SIZE> buffer;
		udp::endpoint main_endpoint;
		connector.receive_from(boost::asio::buffer(buffer), main_endpoint);
		for(int x = 0; x < BUFFER_SIZE; x++){
				if(buffer.data()[x].get_flag() == ENGINE_END){
					server_done = true;
				}
				if(buffer.data()[x].get_flag() != UNITIALIZED){
					_eng_access.lock();
					info_list.push_back(buffer.data()[x]);
					_eng_access.unlock();
				}
			}
		}

	// Doesn't work yet
	void stop_server(){
		server_done = true;
		connector.close();
		connector.shutdown(boost::asio::ip::udp::socket::shutdown_both);
		connector.close();
	}

	// Retrieves objects and clears list to prevent large sizes and redundancy
	std::vector<Engine_Info> get_list(){
		_eng_access.lock();
		std::vector<Engine_Info> tmp_list = info_list;
		info_list.clear();
		_eng_access.unlock();
		return tmp_list;
	}

}

#endif
