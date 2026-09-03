#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#include "network_utils.h"
#define BUFFER_SIZE 1024

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Server started." << std::endl;
	
	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	int bindVal = bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));

	if (bindVal == -1) perror("bind");
	listen(fd, 5);
	
	int client_fd = accept(fd, nullptr, nullptr);
	std::cout << "\tClient fd: " << client_fd << std::endl;
	
	uint32_t network_length{};
    ssize_t len_size = read_exact(client_fd, &network_length, sizeof(network_length), sizeof(network_length));

	if (len_size != sizeof(network_length)) {
    	std::cerr << "Failed to read message length." << std::endl;
    	return 1;
	}
	uint32_t message_length = ntohl(network_length);

	std::cout << "\tIncoming message length: " << message_length << std::endl;

	char buffer[BUFFER_SIZE]{};
	ssize_t recv_val = read_exact(client_fd, buffer, BUFFER_SIZE-1, message_length); // last place is reserved for '\0'

	if (recv_val == -1) {
		std::cerr << "Failed to read message." << std::endl;
		return 1;
	} else if (recv_val == 0) {
		std::cout << "Client disconnected." << std::endl;
		return 0;
	} else if (recv_val < message_length) {
		std::cerr << "Client disconnected before the full message was received." << std::endl;
		return 1;
	}

	if (buffer[recv_val-1] == '\n') {
		buffer[recv_val-1] = 0;
		recv_val--;
	}
	
	std::string msg = "Successfully received the data: ";
	std::string response = msg + "\"" + buffer +  "\"" + ".\n";
	uint32_t response_length = (uint32_t) htonl(response.size());

	send_all(client_fd, &response_length, sizeof(response_length));
	send_all(client_fd, response.data(), response.size());

	return 0;
}