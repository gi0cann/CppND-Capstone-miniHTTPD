#include <cstring>
#include <cstdio>
#include <iostream>
#include <sys/time.h>
#include "Client.h"

Client::Client(SOCKET socket, struct sockaddr_storage clientaddr) {
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    _socket = socket;
    setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    client_address = clientaddr;
    _request = "";
}

Client::~Client() {
    std::cout << " client class destructor" << "\n";
    closeSocket();
}

std::string Client::getRequest() {
    int bytes_received, total_bytes = 0;
    char data[1];
    while ((bytes_received = recv(_socket, data, 1, 0)) > 0) {
        total_bytes += bytes_received;

        _request.append(data);

    }
    return _request;
}

void Client::closeSocket() {
    CLOSESOCKET(_socket);
}

int Client::Send(std::string str) {
    return send(_socket, str.c_str(), (int)std::strlen(str.c_str()), 0);
}

SOCKET Client::getSocket() {
    return _socket;
}

std::string Client::getNameInfo() {
    socklen_t client_len = sizeof(client_address);
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address,
            client_len, address_buffer, sizeof(address_buffer), 0, 0,
            NI_NUMERICHOST);
    std::string nameinfo = std::string(address_buffer);
    return nameinfo;
}
