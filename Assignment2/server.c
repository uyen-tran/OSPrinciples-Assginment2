#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <wait.h>

#define PORT 30506

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed.");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt failed.");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address,
			sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	printf("Listening...\n");

    if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while(1) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		pid_t pid = fork();
		if (pid < 0) {
			exit(-1);
		}
		if (pid == 0) {
			int valread;
			char buffer[1024+1] = {0};

			while (1) {
				if ((valread = read(new_socket, buffer, 1024)) <= 0) {
					exit(-1);
				}

				char* arguments[100];
				int sz = 0;
				int last_idx = 0;
				int str_len = valread;
				buffer[str_len] = '\0';
				printf("Command received: %s\n", buffer);
				for (int i = 0; i < str_len; i++ ) {
					if (buffer[i] == ' ') {
						buffer[i] = '\0';
						arguments[sz++] = &buffer[last_idx];
						last_idx = i+1;
					}
				}
				if(last_idx < str_len) {
					arguments[sz++] = &buffer[last_idx];
				}
				arguments[sz++] = NULL;

				if(fork() == 0) {
					dup2(new_socket, 1);
					dup2(new_socket, 2);
					execvp(arguments[0], arguments);
				} else {
					wait(NULL);
					write(new_socket, "\0", 1);
					printf("Result sent.\n");
				}
			}
		} else {
			continue;
		}
	}
	return 0;
}