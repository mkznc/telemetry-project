#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#define BUFFER_SIZE 1024

int main() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Server started." << std::endl;
	
	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	int bindVal = bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));

	if (bindVal)
		perror("bind");
	
	listen(fd, 5);
	
	int client_fd = accept(fd, nullptr, nullptr);
	std::cout << "\tClient fd: " << client_fd << std::endl;
	
	char buffer[BUFFER_SIZE]{};
	
	uint32_t network_length{};
	recv(client_fd, &network_length, sizeof(network_length), 0);
	uint32_t message_length = ntohl(network_length);

	std::cout << "\tIncoming message length: " << message_length << std::endl;

	int recv_val = recv(client_fd, buffer, BUFFER_SIZE, 0);
	if (buffer[recv_val-1] == '\n') {
		buffer[recv_val-1] = 0;
		recv_val--;
	}
	
	std::cout << "\nData received with size " << recv_val << ": " << buffer << std::endl;
	
	std::string msg = "Successfully received the data: ";
	std::string response = msg + "\"" + buffer +  "\"" + ".\n";
	send(client_fd, response.data(), response.size(), 0);

	return 0;
}