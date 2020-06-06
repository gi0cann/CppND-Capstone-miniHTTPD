#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <string>
#include <map>
#include <exception>

struct InvalidHttpRequestException : public std::exception {
    const char *what() const throw () {
        return "Invalid HTTP request.";
    }
};

struct InvalidHttpVersionException : public std::exception {
    InvalidHttpVersionException(std::string version) : _version(version) {}
    const char *what() const throw () {
        std::string msg("Invalid HTTP Version ");
        msg.append(_version);
        msg.append(" . Only HTTP/1.1 is supported.");
        return msg.c_str();
    }
    std::string _version;
};

class HttpRequest {
    
    public:
        HttpRequest() {}
        HttpRequest(std::string request_str);
        ~HttpRequest();

        // getter/setters
        std::string getMethod() { return _method; }
        std::string getPath() { return _path; }
        std::string getURI() { return _uri; }
        std::string getVersion() { return _http_version; }
        std::string getBody() { return _body; }
        std::string getHost() { return _host; }
        std::string getQuery() { return _query; }
        std::map<std::string, std::string> getHeaders() { return _headers; }
        void parseRequest(std::string request_str);

    private:
        std::string _requestStr;
        std::string _method;
        std::string _uri;
        std::string _http_version;
        std::string _path;
        std::map<std::string, std::string> _headers;
        std::string _body;
        std::string _host;
        std::string _query;
            
};
#endif
