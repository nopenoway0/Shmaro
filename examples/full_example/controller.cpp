#include <unistd.h>
#include <signal.h>
#include <monitor_parser.h>
#include <vector>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <boost/array.hpp>
#include <engine_info.h>
#include <monitor_parser.h>
#include <fstream>
#include <boost/asio.hpp>


using boost::asio::ip::udp;

void print_engine(Engine_Info eng1){	
	std::cout << "Engine Info Object- RPM: " << eng1.get_rpm() << "\nEngine Load: " << eng1.get_load();
	std::cout << "\nFuel Status: " << eng1.get_fuel_status() << "\nIntake Temperature: " << eng1.get_intake_tmp();
	std::cout << "\nSpeed: " << eng1.get_speed() << "\nCoolant Temp: " << eng1.get_coolant_tmp() << "\nLong Trim: ";
	std::cout << eng1.get_ltrim() << "\nShort Trim: " << eng1.get_strim() << "\nThrottle: " << eng1.get_throttle() << "\nAirflow: " << eng1.get_airflow();
}

boost::asio::io_service io_service;
udp::resolver resolver(io_service);

int main(int args, char* argv[]){
	std::string ip_addr_string;
	if(args > 1) ip_addr_string = argv[1];
	else ip_addr_string = "127.0.0.1";
	std::cout << "using ip address " << ip_addr_string;
	udp::endpoint main_endpoint(boost::asio::ip::address::from_string(ip_addr_string), 12345);
	udp::socket connector(io_service);
	connector.open(udp::v4());
	std::ifstream file_to_read;
	std::string read_data;
	file_to_read.open("results.txt");
	bool enter_object = false;
	std::string data = "";
	char turn_on_object[] = "FreezeFrame";
	char turn_off_object[] = "Requesting";
	Engine_Info eng1;
	std::vector<std::string> parsed_data;
	boost::array<Engine_Info, 5> buffer_to_send;
	int object_count = 0;
	while(std::getline(file_to_read, read_data)){
		if(strstr(read_data.c_str(), turn_on_object) != NULL){
			enter_object = true;
		}
		if((strstr(read_data.c_str(), turn_off_object) != NULL) && enter_object == true){
			enter_object = false;
			parsed_data = parse_input(data);
			/*for(int x = 0; x < parsed_data.size(); x++){
				std::cout << parsed_data.at(x) << std::endl;
			}*/		
			eng1 = extract_engine(parsed_data);
			buffer_to_send[object_count] = eng1;
			object_count++;
			if(object_count > 4){
				connector.send_to(boost::asio::buffer(buffer_to_send), main_endpoint);
				for(int x = 0; x < buffer_to_send.size(); x++){
					buffer_to_send[x] = Engine_Info();
				}			
			}
			data = "";
		}
		else if(enter_object == true) data += read_data;
	}
	buffer_to_send[0].set_flag(ENGINE_END);
	connector.send_to(boost::asio::buffer(buffer_to_send), main_endpoint);
	connector.close();
	file_to_read.close();

	return 1;
	}