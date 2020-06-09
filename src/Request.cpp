#include "Request.h"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdio>

HttpRequest::HttpRequest(std::string request_str) {
    _requestStr = request_str;
    parseRequest(request_str);
    std::cout << "Request method: " << _method << "\n";
    std::cout << "host: " << _host << "\n";
    std::cout << "query: " << _query << "\n";
    std::cout << "path: " << _path << "\n";
    std::cout << "http version: " << _http_version << "\n";
    std::cout << "Start headers" << "\n";
    for (auto items : _headers) {
        std::cout << items.first << " : " << items.second << "\n";
    }
    std::cout << "End headers" << "\n";
    std::cout << "body: " << _body << "\n";
    std::cout << "uri: " << _uri << "\n";
}

HttpRequest::~HttpRequest() {
    std::cout << "HttpRequest destructor" << "\n";
}

void HttpRequest::parseRequest(std::string request_str) {
    std::string path_query_fragment, version;

    std::istringstream request_stream(request_str);
    std::string line;
    std::getline(request_stream, line);
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
    
    path_query_fragment = line.substr(oldpos+1, pos-oldpos);

    // get http version
    oldpos = pos;
    if (pos == std::string::npos)
        throw InvalidHttpRequestException();
    version = line.substr(oldpos+1, line.size()-oldpos);
    version.erase(std::remove(version.begin(), version.end(), '\r'), version.end());
    version.erase(std::remove(version.begin(), version.end(), '\n'), version.end());
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
        _path = path_query_fragment;
    } else {
        _query = path_query_fragment.substr(
                qmark_pos, path_query_fragment.size()-qmark_pos);
        path_query_fragment.erase(qmark_pos, path_query_fragment.size());
        _path = path_query_fragment;
    }

    // get host
    std::getline(request_stream, line);
    pos = line.find(":");
    _host = line.substr(pos+2, line.size()-pos);
    //std::cout << "host : " << _host << "\n";

    // get header
    int count = 1;
    std::string key, value;
    std::string::size_type key_pos;
    for (; std::getline(request_stream, line);) {
        count++;
        if (line == "\r") {
            break;
        }
        key_pos = line.find(":");
        key = line.substr(0, key_pos);
        key.erase(std::remove(key.begin(), key.end(), '\r'), key.end());
        value = line.substr(key_pos+1, line.size()-key_pos);
        value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
        _headers[key] = value;
    }
    
    // get body
    if (std::getline(request_stream, line)) {
        _body = line;
    } else {
        _body = "";
    }

    _host.erase(std::remove(_host.begin(), _host.end(), '\r'), _host.end());
    _method.erase(std::remove(_method.begin(), _method.end(), '\r'), _method.end());
    _http_version.erase(std::remove(_http_version.begin(), _http_version.end(), '\r'), 
            _http_version.end());
    _path.erase(std::remove(_path.begin(), _path.end(), '\r'), _path.end());
    _path.erase(std::remove(_path.begin(), _path.end(), ' '), _path.end());
    _query.erase(std::remove(_query.begin(), _query.end(), '\r'), _query.end());
    _body.erase(std::remove(_body.begin(), _body.end(), '\r'), _body.end());

    // get uri
    _uri.append("http://");
    _uri.append(_host);
    _uri.append(_path);
    _uri.append(_query);

}
