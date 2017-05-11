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
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

int main(int args, char* argv[]){
	// Instantiate pipes, variables, buffers.
	// Make sure all pipes are created correctly before the fork
	pid_t pid;
	int r_w_pipe[2];
	int w_r_pipe[2];
	int result = pipe2(r_w_pipe, O_NONBLOCK);
	char buffer[10240] = {0};

	if(result == -1){
		std::cerr << "Could not form pipe " << strerror(errno) << std::endl;
		exit(4);
	}

	result = pipe2(w_r_pipe, O_NONBLOCK);

	if(result == -1){
		std::cerr << "Could not form pipe " << strerror(errno) << std::endl;
		exit(4);
	}

	pid = fork();

	//Child process. Is a copy of main program until execl overwites the fork
	if(pid == 0){
		close(r_w_pipe[1]);
		close(w_r_pipe[0]);
		dup2(w_r_pipe[1], STDOUT_FILENO);
		dup2(w_r_pipe[1], STDERR_FILENO);
		dup2(r_w_pipe[0], STDIN_FILENO);
		execl("/usr/local/bin/scantool", "scantool", (char*) NULL);
		return 1;
	}
	// main process
	else{
		boost::asio::io_service io_service;
		udp::endpoint main_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 12345);
		udp::socket connector(io_service);
		connector.open(udp::v6());
		// Close reading end of 1 pair of pipes and the writing end of the other
		close(r_w_pipe[0]);
		close(w_r_pipe[1]);

		//Text print out
		std::cout << "Starting automated environment commands" << std::endl;

		// Write commands to scantool program
		write(r_w_pipe[1], "set interface elm\n", strlen("set interface elm\n"));
		write(r_w_pipe[1], "set port /dev/ttyUSB0\n", strlen("set port /dev/ttyUSB0\n"));
		write(r_w_pipe[1], "scan\n", strlen("scan\n"));

		// Error message of last scan attempt when using ELM interface
		// will need to add other messages for other interfaces
		char error_string[] = 	"Please check :\n\tAdapter is connected to PC\n\tCable is connected to Vehicle\n\tVehicle is switched on\n\tVehicle is OBDII compliant";
		char connected_string[] = "Connection to ECU established";
		// Start read for failed scan. Continue read until nothing is left in buffer
		// If buffer then contains either of the flagger "error codes" - scan has failed.
		// Wait 4 second and try again
		std::cout << "Scanning for connection";
		std::cout.flush();
		int result;
		int count;
		int waiter = 0; // TODO: replace this terrible way to check with select()
		// Prepare for asyncrhonous read operation
		// Essentially times out in 30 seconds
		while(waiter < 30){
			memset(buffer, 0, sizeof(buffer));			
			count = read(w_r_pipe[0], buffer, sizeof(buffer));
			if(count < 1){
				if(errno = EAGAIN){
					waiter += 1;
				}else if(count == 0){
					// Nothing to read - e.g. read 0 bytes
					waiter += 1;
				}
			}
			else{
				waiter = 0;
			}
			std::cout << buffer;
			std::cout << ".";
			std::cout.flush();
			// If while loop isn't broken, error_string was returned. Keep scanning
			if(strstr(buffer, error_string) != NULL){
			//	sleep(3);
				std::cout << std::endl << "No Connection Found. Restarting Scan";
				std::cout.flush();
				write(r_w_pipe[1], "scan\n", strlen("scan\n"));
			}
			else if(strstr(buffer, connected_string) != NULL){
				waiter = 0;
				break;
			}
			sleep(1);
		}
		// scan is either connected or timed out
		if(waiter >= 30){
			//scan failed
			std::cerr << "Scan failed";
		}
		/**
		* Sucessful connection
		* Functionalities to add:
		* ClearTDC on request, pull error codes, trigger monitor command and stream
		*/
		else{
			std::cout << std::endl << "CONNECTION FOUND" << std::endl;
			boost::array<Engine_Info, 5> eng_buff;
			//start scan	
			memset(buffer,0,sizeof(buffer));
			std::vector<std::string> parsed_data;
			std::vector<std::string> relevant_data;
		//	std::vecotr<std::string
			std::cout << "STARTING MONITOR" << std::endl;
			Engine_Info eng;
			//break code for loop
			char error_str3[] = "request failed";
			write(r_w_pipe[1], "monitor\n", strlen("monitor\n"));		
			count = 0;
			while(strstr(buffer, error_str3) == NULL){
				memset(buffer,0,sizeof(buffer));
				relevant_data.clear();
				read(w_r_pipe[0], buffer, sizeof(buffer));
				//memset(buffer,0,sizeof(buffer));
				parsed_data = parse_input(buffer);
				for(int x = 0; x < parsed_data.size(); x++){
					if(strstr(parsed_data.at(x).c_str(), "...") == NULL && strstr(parsed_data.at(x).c_str(), ":") == NULL){
						std::cout << x << ": " << parsed_data.at(x) << std::endl;
						relevant_data.push_back(parsed_data.at(x));	
					}
				}
				std::cout << ".";
				std::cout.flush();
				eng = extract_engine(relevant_data);
				//if(relevant_data.size() > 0) std::cout << "Printing Engine\n" << "RPM: " << eng.get_rpm() << "\nLoad: " << eng.get_load() << std::endl;
				eng_buff[count] = eng;
				count += 1;
				if(count > 4){
					connector.send_to(boost::asio::buffer(eng_buff), main_endpoint);
					for(int x = 0; x < eng_buff.size(); x++){
						eng_buff[x] = Engine_Info();
					}
				}
				sleep(1);
			}
			// Pull information from terminal here:
			// Use parser?	
			// get error codes using "pids" command
			// ask to clear dtc(optional)
		}
		//quitting here
		std::cout << std::endl<< "Shutting down Scantool";
		while(getpgid(pid) >= 0){
			write(r_w_pipe[1], "quit\n", strlen("quit\n"));
			std::cout << ".";
			std::cout.flush();
			sleep(1);
		}
		std::cout << std::endl;

	}
	return 1;
}
