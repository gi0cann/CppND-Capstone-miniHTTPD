#include <iostream>
#include <memory>
#include <cstring>
#include "Server.h"

Server::Server(std::string port) {
    _port = port;
    createSocket();
}

Server::~Server() {
    std::cout << "server class destructor" << "\n";
    closeSocket();
}

void Server::createSocket() {
    // Configure local address
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo("0.0.0.0", _port.c_str(), &hints, &bind_address);

    // Create socket
    _socket = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(_socket)) {
        std::cerr << "Failed to create server socket. Error: " 
            << GETSOCKETERRNO() << "\n";
        throw ServerSocketException();
    }

    // Make local address reusable
    int option = 1;
    if (setsockopt(_socket, SOL_SOCKET, 
                SO_REUSEADDR, (void*)&option, sizeof(option)) < 0) {
        std::cerr << "Failed to set SO_REUSEADDR. Error: " 
            << GETSOCKETERRNO() << "\n";
        throw ServerSetsockoptException();
    }

}

bool Server::Bind() {
    if (bind(_socket, bind_address->ai_addr, 
                (int)bind_address->ai_addrlen) < 0) {
        std::cerr << "Failed to bind server socket. Error: " 
            << GETSOCKETERRNO() << "\n";
        return false;
    }
    freeaddrinfo(bind_address);
    return true;
}

bool Server::Listen(int backlog) {
    // Listen
    if (listen(_socket, backlog) < 0) {
        std::cerr << "Failed to listen on port " << _port << ". Error: " 
            << GETSOCKETERRNO() << "\n";
        return false;
    }
    std::cout << "Listening on port " << _port << "\n";
    return true;
}

std::unique_ptr<Client> Server::Accept(struct sockaddr_storage &client_address) {
    socklen_t client_len = sizeof(client_address);
    SOCKET client_socket = accept(_socket, 
            (struct sockaddr*) &client_address, &client_len);
    std::unique_ptr<Client> csocket = std::make_unique<Client>(client_socket, client_address);
    return csocket;
}

void Server::closeSocket() {
    CLOSESOCKET(_socket);
}
