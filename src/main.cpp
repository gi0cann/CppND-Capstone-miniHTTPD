#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#include <iostream>
#include <cstring>
#include <string>

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif


int main() {

    std::string port = "8089";

// Initialize Winsock
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        std::cerr << "Failed to initialize.\n" << "\n";
        return 1;
    }
#endif
    
    std::cout << "Ready to use socket API.\n" << "\n";

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

    // Bind socket
    if (bind(server_socket,
                bind_address->ai_addr, (int)bind_address->ai_addrlen) < 0) {
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

// Cleanup Winsock
#if defined(_WIN32)
    WSACleanup();
#endif


    return 0;


}
