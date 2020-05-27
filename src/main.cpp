#include <iostream>
#include <string>
#include <cstring>
#include <csignal>

#include "portable_sockets.h"

void signal_handler(int signal) {
    // Cleanup Winsock
    cleanup_sockets();

    std::cout << "Bye..." << "\n";
    exit(signal);
}

int main() {

    std::string port = "8089";

    // register SIGINT handler
    signal(SIGINT, signal_handler);

    // Initialize Winsock
    if (!init_sockets()) {
        return 1;
    }
    
    std::cout << "Ready to use socket API." << "\n";

    // Configure local address
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, port.c_str(), &hints, &bind_address);

    // Create socket
    SOCKET server_socket;
    server_socket = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(server_socket)) {
        std::cerr << "Failed to create server socket. Error: " 
            << GETSOCKETERRNO() << "\n";
        return 1;
    }

    // Make local address reusable
    int option = 1;
    if (setsockopt(server_socket, SOL_SOCKET, 
                SO_REUSEADDR, (void*)&option, sizeof(option)) < 0) {
        std::cerr << "Failed to set SO_REUSEADDR. Error: " 
            << GETSOCKETERRNO() << "\n";
        return 1;
    }

    // Bind socket
    if (bind(server_socket, bind_address->ai_addr, 
                (int)bind_address->ai_addrlen) < 0) {
        std::cerr << "Failed to bind server socket. Error: " 
            << GETSOCKETERRNO() << "\n";
        return 1;
    }
    freeaddrinfo(bind_address);

    // Listen
    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on port " << port << ". Error: " 
            << GETSOCKETERRNO() << "\n";
        return 1;
    }
    std::cout << "Listening on port " << port << "\n";

    // Server loop. Handle incoming connections in an infinite loop.
    while (true) {

        // Accept incoming connections
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET client_socket = accept(server_socket, 
                (struct sockaddr*) &client_address, &client_len);
        if (!ISVALIDSOCKET(client_socket)) {
            std::cerr << "Failed to accept incomming client connection. Error: " 
                << GETSOCKETERRNO() << "\n";
            continue;
        }

        // Display client connection
        char address_buffer[100];
        getnameinfo((struct sockaddr*)&client_address,
                client_len, address_buffer, sizeof(address_buffer), 0, 0,
                NI_NUMERICHOST);
        std::cout << "Client connected from " << address_buffer << ".\n"; 
        
        // Get data from client
        char request[1024];
        int bytes_received = recv(client_socket, request, 1024, 0);
        std::cout << "Received " << bytes_received << " bytes from " 
            << address_buffer << ".\n";
        
        // Send response to client
        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "<h1>Welcome to MiniHTTPD!</h>";
        int bytes_sent = send(client_socket, response, (int)std::strlen(response), 0);
        std::cout << "Sent " << bytes_sent << " of " 
            << std::strlen(response) << " bytes.\n";
        
        std::cout << "Closing client(" << address_buffer << ") connection.\n";

        // Close client socket
        CLOSESOCKET(client_socket);

    }
    
    // Close server socket
    CLOSESOCKET(server_socket);


    return 0;

}
