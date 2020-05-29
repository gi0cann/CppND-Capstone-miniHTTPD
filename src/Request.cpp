#include "Request.h"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

HttpRequest::HttpRequest(std::string request_str) {
    parseRequest(request_str);
    std::cout << "Request method: " << _method << "\n";
}

HttpRequest::~HttpRequest() {
    std::cout << "HttpRequest destructor" << "\n";
}

void HttpRequest::parseRequest(std::string request_str) {
    std::string path_query_fragment, version;

    std::istringstream request_stream(request_str);
    std::string line;
    std::getline(request_stream, line);
    std::cout << "first getline: " << line << "\n";
    std::string::size_type pos;
    // get Request method
    pos = line.find(" ");
    if (pos == std::string::npos)
        throw InvalidHttpRequestException();

    _method = line.substr(0, pos);

    // get Path and query string
    std::string::size_type oldpos = pos;
    pos = line.find(" ", oldpos+1);
    if (pos == std::string::npos)
        throw InvalidHttpRequestException();
    
    path_query_fragment = line.substr(oldpos, pos-oldpos);
    std::cout << "oldpos:" << oldpos << " pos:" << pos << "\n";
    std::cout << "path_query_frament: " << path_query_fragment << "\n";

    // get http version
    oldpos = pos;
    if (pos == std::string::npos)
        throw InvalidHttpRequestException();
    version = line.substr(oldpos+1, line.size()-oldpos);
    version.erase(std::remove(version.begin(), version.end(), '\r'), version.end());
    version.erase(std::remove(version.begin(), version.end(), '\n'), version.end());
    std::cout << "version: " << version << "\n";
    if (version != "HTTP/1.1")
        throw InvalidHttpVersionException(version);
    _http_version = version;

    // get query string
    std::string::size_type fragment_pos = path_query_fragment.find("#");
    if (fragment_pos != std::string::npos) {
        path_query_fragment.erase(fragment_pos, path_query_fragment.size());
    }
    std::string::size_type qmark_pos = path_query_fragment.find("?");
    if (qmark_pos == std::string::npos) {
        _query = "";
    } else {
        _query = path_query_fragment.substr(
                qmark_pos, path_query_fragment.size()-qmark_pos);
        path_query_fragment.erase(qmark_pos, path_query_fragment.size());
        _path = path_query_fragment;
    }
    std::cout << "query: " << _query << "\n";
    std::cout << "path: " << _path << "\n";

    // get path

    std::cout << "\n" << "Request loop" << "\n";
    int count = 1;
    for (line; std::getline(request_stream, line);) {
        count++;
        std::cout << "line " << count << ":" << line << "\n";
    }

}
