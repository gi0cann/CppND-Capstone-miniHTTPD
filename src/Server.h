#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <memory>
#include <exception>
#include "portable_sockets.h"
#include "Client.h"

struct ServerSocketException : public std::exception {
    const char *what() const throw () {
        return "Server - Failed to create socket.";
    }
};

struct ServerSetsockoptException : public std::exception {
    const char *what() const throw () {
        return "Server - setsockopt error.";
    }
};

class Server {

    public:
        // constructors/destructor
        Server(std::string port);
        ~Server();

        void createSocket();
        std::unique_ptr<Client> Accept(struct sockaddr_storage &client_address);
        bool Bind();
        bool Listen(int backlog);
        void closeSocket();

    private:
        SOCKET _socket;
        std::string _port;
        struct addrinfo *bind_address;

};
#endif
