#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define BUFFER_SIZE 100

int Socket(int domain, int type, int protocol) {
	int socket_fd;

	if ((socket_fd = socket(domain, type, protocol)) < 0) {
		printf("Failed to create a socket!\n");
		exit(1);
	}
	
	return socket_fd;
}

void Bind(int socket, const struct sockaddr *address, socklen_t address_len) {
	if (bind(socket, address, address_len) < 0) {
		printf("Failed to bind socket\n");
		exit(1);
	}
}

int main(int argc, char **argv) {
	int socket_fd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr, client_addr;
	int client_struct_length = sizeof(client_addr);

	memset(buffer, '\0', BUFFER_SIZE);
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	memset(&client_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT); 
 	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	// create a UDP socket
	socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

	// bind that socket to our port of choice
	Bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	

	printf("Server listening on port %d...\n", PORT);

	while (1) {
		if (recvfrom(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, (socklen_t *)&client_struct_length) < 0) {
			printf("Failed to receive\n");
			return 1;
		}

		printf("From client: %s", buffer);
		memset(buffer, '\0', BUFFER_SIZE);
	}

	close(socket_fd);

	return 0;
}
