#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include <string>
#include <map>
#include "Request.h"
#include "Client.h"

class HttpResponse {

    public:
        HttpResponse() {}
        HttpResponse(HttpRequest &request);
        std::string generateResponse();
        std::string response200 = "HTTP/1.1 200 OK\r\n";
        std::string response400 = "HTTP/1.1 400 Bad Request\r\n";
        std::string response404 = "HTTP/1.1 404 Not Found\r\n";
        std::string response500 = "HTTP/1.1 500 Internal Server Error\r\n";
        std::string response505 = "HTTP/1.1 505 HTTP Version Not Supported\r\n";

    private:
        std::map<std::string, std::string> _headers;
        std::string _responseCode;
        std::string _body;
        std::string _docroot = "htdocs";
        std::string _defaultPage = "/index.html";
};
#endif
