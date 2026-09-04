CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic

BUILD_DIR := build
SERVER := $(BUILD_DIR)/server
CLIENT := $(BUILD_DIR)/client

COMMON_SRC := network_utils.cpp

.PHONY: build run clean

build:
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) server.cpp $(COMMON_SRC) -o $(SERVER)
	$(CXX) $(CXXFLAGS) client.cpp $(COMMON_SRC) -o $(CLIENT)

run: build
	./$(SERVER) & \
	SERVER_PID=$$!; \
	sleep 0.2; \
	./$(CLIENT); \
	wait $$SERVER_PID

clean:
	rm -rf $(BUILD_DIR)