#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

#include "network_utils.h"
#define RSP_SIZE 1024

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int connectVal = connect(fd, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address));
    if (connectVal != 0)
        perror("connect");

    std::string msg = "hello from client!";
    uint32_t network_length = (uint32_t) htonl(msg.size());

    send_all(fd, &network_length, sizeof(network_length));
    send_all(fd, msg.data(), msg.size());

    uint32_t response_length{};
    read_exact(fd, &response_length, sizeof(response_length), sizeof(response_length));
	uint32_t message_length = ntohl(response_length);

    char response[RSP_SIZE]{};
    read_exact(fd, response, RSP_SIZE, message_length);
    
    std::cout << "Response returned: " << response << std::endl;
    return 0;
}