#include <iostream>
#include <string>
#include <cstring>
#include <csignal>

#include "portable_sockets.h"
#include "Server.h"
#include "Client.h"

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
        Client client_socket = server.Accept(client_address);
        if (!ISVALIDSOCKET(client_socket.getSocket())) {
            std::cerr << "Failed to accept incomming client connection. Error: " 
                << GETSOCKETERRNO() << "\n";
            continue;
        }

        // Display client connection
        std::cout << "Client connected from " << client_socket.getNameInfo() << ".\n"; 
        
        // Get data from client
        std::string request = client_socket.getRequest();
        std::cout << "\nStart request" << "\n";
        std::cout << request << "\n";
        std::cout << "End request" << "\n\n";
        
        // Send response to client
        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "<h1>Welcome to MiniHTTPD!</h>";
        std::string responseStr = std::string(response);
        int bytes_sent = client_socket.Send(responseStr);
        std::cout << "Sent " << bytes_sent << " of " 
            << std::strlen(response) << " bytes.\n";
        
        std::cout << "Closing client(" << client_socket.getNameInfo() << ") connection.\n";

        // Close client socket
        client_socket.closeSocket();

    }
    
    // Close server socket
    server.closeSocket();


    return 0;

}
