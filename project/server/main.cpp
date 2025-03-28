#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <random>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    const char* portEnv = std::getenv("CPP_SERVER_PORT");
    int port = portEnv ? std::stoi(portEnv) : 5000; // default 5000

    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Bind to address
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options\n";
        return 1;
    }

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "C++ Server listening on port " << port << "...\n";

    while (true) {
        int addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        // Read message from client
        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));
        int read_bytes = read(client_socket, buffer, 1023);
        if (read_bytes > 0) {
            std::string message(buffer);
            // We expect something like "random n"
            if (message.rfind("random", 0) == 0) {
                // Extract n
                std::istringstream iss(message);
                std::string cmd;
                int n;
                iss >> cmd >> n; // cmd should be "random", then n

                // Generate n random numbers from a normal distribution
                std::random_device rd;
                std::mt19937 gen(rd());
                std::normal_distribution<double> dist(0.0, 1.0);

                std::ostringstream response;
                for (int i = 0; i < n; ++i) {
                    double val = dist(gen);
                    response << val;
                    if (i < n - 1) {
                        response << ",";
                    }
                }
                response << "\n"; // end with newline

                std::string respStr = response.str();
                send(client_socket, respStr.c_str(), respStr.size(), 0);
            } else {
                // Unknown command
                std::string respStr = "Unknown command\n";
                send(client_socket, respStr.c_str(), respStr.size(), 0);
            }
        }
        close(client_socket);
    }

    close(server_fd);
    return 0;
}
