#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

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

		// Start read for failed scan. Continue read until nothing is left in buffer
		// If buffer then contains either of the flagger "error codes" - scan has failed.
		// Wait 4 second and try again
		std::cout << "Scanning for connection";
		std::cout.flush();
		int result;
		int count;
		int waiter = 0; // TODO: replace this terrible way to check with select()
		// Prepare for asyncrhonous read operation
		while(waiter < 5 || (strstr(buffer, error_string) == NULL)){
			count = read(w_r_pipe[0], buffer, sizeof(buffer));
			if(count < 1){
				if(errno = EAGAIN){
					waiter += 1;
				}
			}
			else{
				waiter = 0;
			}
			std::cout << ".";
			std::cout.flush();
			// If while loop isn't broken, error_string was returned. Keep scanning
			write(r_w_pipe[1], "scan\n", strlen("scan\n"));
			sleep(1);
		}
		// scan is either connected or timed out
		if(waiter >=5){
			//scan failed
		}
		/**
		* Sucessful connection
		* Functionalities to add:
		* ClearTDC on request, pull error codes, trigger monitor command and stream
		*/
		else{
			std::cout << std::endl << "Connection Found" << std::endl;
			// get error codes using "pids" command
			// ask to clear dtc(optional)
			// start realtime monitoring
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
