#include <iostream>
#include <string>
#include <cstring>
#include <csignal>
#include <thread>
#include <fstream>
#include <future>

#include "portable_sockets.h"
#include "Server.h"
#include "Client.h"
#include "Request.h"
#include "Response.h"

void signal_handler(int signal) {
    // Cleanup Winsock
    cleanup_sockets();

    std::cout << "Bye..." << "\n";
    exit(signal);
}

void request_handler(Client &client) {
    HttpRequest requestObj;
    HttpResponse responseObj;
    // Display client connection
    std::cout << "Client connected from " << client.getNameInfo() << ".\n"; 
    
    // Get data from client
    std::string request = client.getRequest();
    SOCKET _s = client.getSocket();
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
        int bytes_sent = client.Send(responseStr);
        std::cout << "Sent " << bytes_sent << " of " 
            << std::strlen(response) << " bytes.\n";
        
        std::cout << "Closing client(" << client.getNameInfo() << ") connection.\n";

        // Close client socket
        client.closeSocket();

    } catch(InvalidHttpVersionException& e) {
        std::cout << e.what() << "\n";
        const char *response =
            "HTTP/1.1 505 HTTP Version Not Supported\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "<h1>This web server only supports HTTP/1.1</h>";
        std::string responseStr = std::string(response);
        int bytes_sent = client.Send(responseStr);
        std::cout << "Sent " << bytes_sent << " of " 
            << std::strlen(response) << " bytes.\n";
        
        std::cout << "Closing client(" << client.getNameInfo() << ") connection.\n";

        // Close client socket
        client.closeSocket();
        
    }
    
    //std::this_thread::sleep_for(std::chrono::seconds(20));
    
    // Send response to client
    std::string response = responseObj.generateResponse();
    int bytes_sent = client.Send(response);
    std::cout << "Sent " << bytes_sent << " of " 
        << response.size() << " bytes.\n";
    
    std::cout << "Closing client(" << client.getNameInfo() << ") connection.\n";

    // Close client socket
    client.closeSocket();

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
        Client client_socket = server.Accept(client_address);
        SOCKET _s = client_socket.getSocket();
        printf("socket ponter: %p", &_s);
        if (!ISVALIDSOCKET(client_socket.getSocket())) {
            std::cerr << "Failed to accept incomming client connection. Error: " 
                << GETSOCKETERRNO() << "\n";
            continue;
        }
    
        request_handler(client_socket);

    }


     
    // Close server socket
    server.closeSocket();

    return 0;

}
