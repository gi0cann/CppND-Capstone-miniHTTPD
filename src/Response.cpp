#include <string>
#include <fstream>
#include "Response.h"
#include "Request.h"

HttpResponse::HttpResponse(HttpRequest &request) {
    std::ifstream responseStream;
    if (request.getPath() == "/") {
        responseStream = std::ifstream(_docroot + _defaultPage);
    } else {
        responseStream = std::ifstream(_docroot + request.getPath());
    }

    if (responseStream.is_open()) {
        std::string line;
        _responseCode = response200;
        _headers["Connection"] = "close";
        _headers["Content-Type"] = "text/html";
        while (std::getline(responseStream, line)) {
            _body.append(line);
        }
        responseStream.close();
    } else {
        _responseCode = response404;
        _headers["Connection"] = "close";
        _headers["Content-Type"] = "text/html";
        responseStream.close();
    }
}

std::string HttpResponse::generateResponse() {
    std::string response;
    response.append(_responseCode);
    for (auto items : _headers) {
        response.append(items.first + " : " + items.second + "\r\n");
    }
    response.append("\r\n");
    response.append(_body);
    return response;
}
