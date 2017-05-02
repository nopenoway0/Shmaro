#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

int main(int args, char* argv[]){
	pid_t pid;
	int r_w_pipe[2];
	int result = pipe(r_w_pipe);
	if(result == -1){
		std::cerr << "Could not form pipe " << strerror(errno) << std::endl;
		exit(4);
	}
	pid = fork();
	if(pid == 0){
		close(r_w_pipe[1]);
		dup2(r_w_pipe[0], STDIN_FILENO);
		execl("/usr/local/bin/scantool", "scantool", (char*) NULL);
		exit(0);
	}
	else{
		close(r_w_pipe[0]);
		std::cout << "Starting automated environment commands" << std::endl;
		sleep(2);
		write(r_w_pipe[1], "set interface elm\n", strlen("set interface elm\n"));
		write(r_w_pipe[1], "set port /dev/ttyUSB0\n", strlen("set port /dev/ttyUSB0\n"));
		write(r_w_pipe[1], "scan\n", strlen("scan\n"));
		//handling here
		//
		//quitting here
		while(getpgrid(pid) >= 0){
			write(r_w_pipe[1], "quit\n", strlen("quit\n"));	
			sleep(1);
		}

	}
	exit(0);
}