#include <iostream>
#include <string>
#include <cstring>
#include <csignal>

#include "portable_sockets.h"
#include "Server.h"

void signal_handler(int signal) {
    // Cleanup Winsock
    cleanup_sockets();

    std::cout << "Bye..." << "\n";
    exit(signal);
}

int main() {

    std::string portstr = "8089";

    // register SIGINT handler
    signal(SIGINT, signal_handler);

    // Initialize Winsock
    if (!init_sockets()) {
        return 1;
    }
    
    // Create Server object
    Server server = Server(portstr);

    // Bind socket
    if (!server.Bind()) {
        return 1;
    }

    // Listen
    if (!server.Listen(10)) {
        return 1;
    }

    // Server loop. Handle incoming connections in an infinite loop.
    while (true) {

        // Accept incoming connections
        struct sockaddr_storage client_address;
        socklen_t client_len = sizeof(client_address);
        SOCKET client_socket = server.Accept(client_address);
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
    server.closeSocket();


    return 0;

}
