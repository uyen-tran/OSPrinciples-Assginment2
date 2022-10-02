#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 30506

int main(int argc, char const *argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	printf("Successfully connected.\n");
	int valread;
    char cmd[1024] = {};
	char buffer[1024] = {0};

	while (1) {
		printf("user> ");
		scanf(" %[^\n]s", cmd);
		if ((strcmp(cmd,"quit")) == 0) {
			return 0;
		}
		if(strlen(cmd) == 0) {
			continue;
		}
		send(sock, cmd, strlen(cmd) , 0 );
		while((valread = read(sock, buffer, 1024)) > 0) {
			int term = (buffer[valread-1] == '\0');
			valread--;
			fwrite(buffer, 1, valread, stdout);
			if(term) {
				printf("\n");
				break;
			}
		}
		//printf("\n");
	}
	
	return 0;
}
