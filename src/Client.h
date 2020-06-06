#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <map>
#include "portable_sockets.h"
#include "Request.h"

class Client {

    public:
        Client(SOCKET socket, struct sockaddr_storage clientaddr);
        ~Client();
        std::string getRequest();
        void closeSocket();
        int Send(std::string str);
        SOCKET getSocket();
        std::string getNameInfo();

    private:
        SOCKET _socket;
        std::string _request;
        struct sockaddr_storage client_address;
};
#endif
