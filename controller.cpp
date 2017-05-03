#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

int main(int args, char* argv[]){
	pid_t pid;
	int r_w_pipe[2];
	int w_r_pipe[2];
	int result = pipe(r_w_pipe);
	result = pipe(w_r_pipe);
	char buffer[10240] = {0};
	if(result == -1){
		std::cerr << "Could not form pipe " << strerror(errno) << std::endl;
		exit(4);
	}
	pid = fork();
	if(pid == 0){
		close(r_w_pipe[1]);
		close(w_r_pipe[0]);
		dup2(w_r_pipe[1], STDOUT_FILENO);
		dup2(r_w_pipe[0], STDIN_FILENO);
		execl("/usr/local/bin/scantool", "scantool", (char*) NULL);
		exit(0);
	}
	else{
		close(r_w_pipe[0]);
		close(w_r_pipe[1]);
		std::cout << "Starting automated environment commands" << std::endl;
		sleep(2);
		write(r_w_pipe[1], "set interface elm\n", strlen("set interface elm\n"));
		write(r_w_pipe[1], "set port /dev/ttyUSB0\n", strlen("set port /dev/ttyUSB0\n"));
		write(r_w_pipe[1], "scan\n", strlen("scan\n"));
		//handling here
		char flagger[] = "Please check :";
		char flagger2[] = "ntool.c:1535: Unspecified";
		read(w_r_pipe[0], buffer, sizeof(buffer));
		//quitting here
		while(getpgid(pid) >= 0){
			std::cout << std::endl << "Attempting to quit" << std::endl;
			write(r_w_pipe[1], "quit\n", strlen("quit\n"));	
			sleep(1);
		}

	}
	exit(0);
}
