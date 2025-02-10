#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 5000
#define MAX_MESSAGE 100

int Socket(int domain, int type, int protocol) {
	int socket_fd;

	if ((socket_fd = socket(domain, type, protocol)) < 0) {
		printf("Failed to create a socket!\n");
		exit(1);
	}
	
	return socket_fd;
}

int main(int argc, char **argv) {
	int socket_fd;
	char message[MAX_MESSAGE];
	struct sockaddr_in server_addr;

	memset(message, '\0', MAX_MESSAGE);
	memset(&server_addr, 0, sizeof(struct sockaddr_in));

	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) != 1) {
		printf("Unable to parse IP address\n");
		exit(1);
	}

	// create a UDP socket
	socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

	// associate the server address with this socket
	if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to connect to server\n");
	}

	int loop = 1;

	while (loop) {
		printf("> ");

		if (fgets(message, MAX_MESSAGE, stdin) == NULL) {
			printf("Failed to read the message!\n");
			return 1;
		}

		if (strcmp(message, "exit\n") == 0) {
			loop = 0;
		} else {
			if (sendto(socket_fd, message, strlen(message), 0, (struct sockaddr *)NULL, sizeof(server_addr)) < 0) {
				printf("Failed to send the message :(\n");
				return 1;
			}
		}
	}

	close(socket_fd);

	return 0;
}
