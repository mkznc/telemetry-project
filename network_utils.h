#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <cstddef>
#include <sys/types.h>

ssize_t read_exact(int fd, void* buffer, const std::size_t buffer_size, std::size_t size);
ssize_t send_all(int fd, const void* buffer, std::size_t size);

#endif