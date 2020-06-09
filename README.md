# CPPND: Capstone MiniHTTPD

This is a web server supporting HTTP/1.1 written in C++.

## Project Structure
MiniHTTPD is a minimal HTTP (HyperText Transfer Protocol) implement. It supports handling GET request from an HTTP client like a web browser. It will parse incoming GET requests and generate the correct response based on the requested resource.

The MiniHTTPD code consists of the following classes: Server, Client, HttpRequest, HttpResponse, ServerSocketException, ServerSetsockoptException, InvalidHttpRequestException, InvalidHttpVersionException.

### main.cpp
This is the entrypoint to the program. This file mainly consists of the main function, the request_handler function, and signal_handler function. The first thing the main function does is register a SIGINT (CTRL-Z) signal_handler to allows us to do some cleanup if we need to break out of the infinite server loop. It then proceeds to take care of cross-platform socket initialization by calling the init_sockets function defined in portable_sockets.h. It then define the port number for our server to listen on and create Server socket. The Server socket is then used bind and listen on the port number we previously picked. The main function then starts an infinite loop that takes care of the following:

1. The Accept method from the previously created Server object is called to accept incoming socket connection. The Accept method will create a new Client object for each incoming socket connection.
2. A new thread is created calling the request_handler function and the thread is added to the thread queue. The request_handler function takes a Client unique_ptr as its argument and uses this to handle incoming HTTP requests.
3. Once the thread count reaches 50 the threads in the queue joined and the thread queue is cleared.

### portable_sockets.h and portable_sockets.cpp
This contains helper functions and macros for initializing (init_sockets) and cleaning (cleanup_sockets) up sockets for both Windows and *nix operating systems.

### Server.h and Server.cpp
These files contain the Server, ServerSocketException, ServerSetsockoptException classes. The ServerSocketException inherits from std::exception and is used to throw an exception if there is a failure to create a Server socket. The ServerSetsockoptException inherits from std::exception and is used to throw an exception if there is a failure while calling setsockopt on a socket. The Server class is used to create a listening socket to accept incoming connections. The Server class constructor takes the server port number as a string and calls Server::createSocket to create a socket. Server::Bind and Server::Listen are used to bind a socket to listen on the provided port number.

### Client.h and Client.cpp
These file contain the Client class. The Client class constructor takes a SOCKET and a struct sockaddr_storage as arguments an uses them to create a Client object containing information about incoming client connections. Client::getRequest read incoming requests to the server an uses them to create HttpRequest objects that are later used to create HttpResponse objects to send back to the client using Client::Send. Client::getSocket is used to access the _socket property. Client::getNameInfo is used to get a clients IP address.

### HttpRequest.h and HttpRequest.cpp
This files contains the HttpRequest, InvalidHttpRequestException, InvalidHttpVersionException classes. InvalidHttpRequestException inherits from std::exception and is used to throw an exception if an incoming request from a client is not a valid HTTP request. InvalidHttpVersionException inherits from std::exception and is used to throw an exception if an incoming HTTP request is not version HTTP/1.1. HttpRequest class has two constructors, a empty constructor and a constructor that takes a string as an argument and calls HttpRequest::parseRequest to parse incoming HTTP requests. The HttpRequest class also contains the getters getMethod, getPath, getURI, getVersion, getBody, getHost, getQuery, getHeaders that are used to retreive the properties _requestStr, _method, _uri, _http_version, _path, _headers, _path, _body, _host, and _query.

### HttpResponse.h and HttpResponse.cpp
These files contain the HttpResponse class. The HttpResponse class constructor take a reference to a  HttpRequest object and used it to create an HTTP response to send to connecting client. HttpResponse::generateResponse is used to generate a string from HttpResponse object.


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac) 
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
* cl & msbuild (Windows)
  * Windows: recommend using [MSVC 2019](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)

## Rubric items

### The project demonstrates an understanding of C++ functions and control structures.
src/main.cpp line 127 - 149. The application makes use of an infinite while loop to accept incoming connections.

### The project reads data from a file and process the data, or the program writes data to a file.
src/HttpResponse.cpp line 7 - 29. The HttpResponse class constructor reads data from a file and adds it to the response body.

### The project uses Object Oriented Programming techniques.
src/HttpResponse.h
src/HttpResponse.cpp
src/HttpRequest.h
src/HttpRequest.cpp
src/Client.h
src/Client.cpp
src/Server.h
src/Server.cpp

### Classes use appropriate access specifiers for class members
src/HttpResponse.h
src/HttpResponse.cpp
src/HttpRequest.h
src/HttpRequest.cpp
src/Client.h
src/Client.cpp
src/Server.h
src/Server.cpp

### Classes abstract implementation details from their interfaces.
src/HttpResponse.h
src/HttpResponse.cpp
src/HttpRequest.h
src/HttpRequest.cpp
src/Client.h
src/Client.cpp
src/Server.h
src/Server.cpp

### Classes encapsulate behavior.
src/HttpResponse.h
src/HttpResponse.cpp
src/HttpRequest.h
src/HttpRequest.cpp
src/Client.h
src/Client.cpp
src/Server.h
src/Server.cpp

### Overloaded functions allow the same function to operate on different parameters.
src/HttpResponse.h line 11 - 12

### The project makes use of references in function declarations.
src/Server.cpp line 68 - 74
src/Response.cpp line 6 - 29

### The project uses destructors appropriately.
src/Server.cpp line 11 - 14
src/Client.cpp line 17 - 20

### The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
src/Server.cpp line 11 - 14

### The project uses move semantics to move data, instead of copying it, where possible.
src/main.cpp line 140

### The project uses smart pointers instead of raw pointers.
src/main.cpp line 131
src/Server.cpp line 68 - 74

### The project uses multithreading.
src/main.cpp line 140 - 147

### A mutex or lock is used in the project.
src/main.cpp line 17
src/main.cpp line 31

## Basic Build Instructions

### Linux, Mac
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./minihttpd`.

### Windows
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && msbuild minihttpd.sln /p:Configuration=Release`
4. Run it: `Release\minihttpd.exe`.
