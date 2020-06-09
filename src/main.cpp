#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <thread>
#include <fstream>
#include <memory>
#include <vector>
#include <mutex>

#include "portable_sockets.h"
#include "Server.h"
#include "Client.h"
#include "Request.h"
#include "Response.h"

std::mutex mtx;

void signal_handler(int signal) {
    // Cleanup Winsock
    cleanup_sockets();
    exit(signal);
}

void request_handler(std::unique_ptr<Client> client) {
    HttpRequest requestObj;
    HttpResponse responseObj;
    std::lock_guard<std::mutex> lck(mtx);
    // Display client connection
    std::cout << "Client connected from " << client->getNameInfo() << ".\n"; 
    
    // Get data from client
    std::string request = client->getRequest();
    SOCKET _s = client->getSocket();
    printf("socket ponter: %p", &_s);
    std::cout << "\nStart request" << "\n";
    std::cout << request << "\n";
    std::cout << "End request" << "\n\n";
    
    try {
        requestObj = HttpRequest(request);
        responseObj = HttpResponse(requestObj);
    } catch(InvalidHttpRequestException& e) {
        std::cout << e.what() << "\n";
        const char *response =
            "HTTP/1.1 400 Bad Request\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "<h1>Your client issued an illegal request</h>";
        std::string responseStr = std::string(response);
        int bytes_sent = client->Send(responseStr);
        std::cout << "Sent " << bytes_sent << " of " 
            << std::strlen(response) << " bytes.\n";
        
        std::cout << "Closing client(" << client->getNameInfo() << ") connection.\n";

        // Close client socket
        client->closeSocket();

    } catch(InvalidHttpVersionException& e) {
        std::cout << e.what() << "\n";
        const char *response =
            "HTTP/1.1 505 HTTP Version Not Supported\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "<h1>This web server only supports HTTP/1.1</h>";
        std::string responseStr = std::string(response);
        int bytes_sent = client->Send(responseStr);
        std::cout << "Sent " << bytes_sent << " of " 
            << std::strlen(response) << " bytes.\n";
        
        std::cout << "Closing client(" << client->getNameInfo() << ") connection.\n";

        // Close client socket
        client->closeSocket();
        
    }
    
    // Send response to client
    std::string response = responseObj.generateResponse();
    int bytes_sent = client->Send(response);
    std::cout << "Sent " << bytes_sent << " of " 
        << response.size() << " bytes.\n";
    
    std::cout << "Closing client(" << client->getNameInfo() << ") connection.\n";

    // Close client socket
    client->closeSocket();
    mtx.unlock();

}

int main() {
    
    // register SIGINT handler
    signal(SIGINT, signal_handler);

    std::string portstr = "8089";


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

    std::vector<std::thread> client_threads;

    // Server loop. Handle incoming connections in an infinite loop.
    while (true) {

        // Accept incoming connections
        struct sockaddr_storage client_address;
        std::unique_ptr<Client> client_socket = server.Accept(client_address);
        if (!ISVALIDSOCKET(client_socket->getSocket())) {
            std::cerr << "Failed to accept incomming client connection. Error: " 
                << GETSOCKETERRNO() << "\n";
            continue;
        }
    
        client_threads.emplace_back(std::thread(request_handler, std::move(client_socket)));

        if (client_threads.size() >= 50) {
            for (auto &c : client_threads) {
                c.join();
            }
            client_threads.clear();
        }

    }


     
    // Close server socket
    server.closeSocket();

    return 0;

}
