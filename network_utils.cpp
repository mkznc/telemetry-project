#include "network_utils.h"
#include <sys/socket.h>

ssize_t read_exact(int fd, void* buffer, const std::size_t buffer_size, std::size_t size) {
    if (buffer == nullptr || buffer_size < size)
        return -1;

    std::size_t received = 0;

    char* byte_buffer = static_cast<char*>(buffer);
    while (received < size) {
        ssize_t result = recv(fd, byte_buffer + received, size - received, 0);
        
        if (result == -1) return -1;
        else if (result == 0) return received;

        received += result;
    }

    return received;
}

ssize_t send_all(int fd, const void* buffer, std::size_t size) {
    if (buffer == nullptr)
        return -1;

    std::size_t sent = 0;

    const char* byte_buffer = static_cast<const char*>(buffer);
    while (sent < size) {
        ssize_t result = send(fd, byte_buffer + sent, size - sent, 0);

        if (result == -1) return -1;
        if (result == 0) return sent;

        sent += result;
    }

    return sent;
}