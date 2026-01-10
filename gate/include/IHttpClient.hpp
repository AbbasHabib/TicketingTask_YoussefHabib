#pragma once

#include <sstream>
#include <string>
#include <map>


struct HttpResponse
{
    long status_code;
    std::string body;
    std::map<std::string, std::string> headers;

    operator std::string()
    {
        std::stringstream headers_str;
        headers_str << "{";
        for(const auto& [key, val] : headers)
        {
            headers_str << "{" << key << ", " << val << "}";
        }
        headers_str << "}";

        return "body: " + body + ", status_code" + std::to_string(status_code)
                + "headers " + headers_str.str();
    }
};

class IHttpClient
{
public:
    virtual ~IHttpClient() = default;

    virtual std::optional<HttpResponse> get(
        const std::string& url,
        const std::string& body,
        const std::map<std::string, std::string>& headers = {}
    ) = 0;

    virtual std::optional<HttpResponse> post(
        const std::string& url,
        const std::string& body,
        const std::map<std::string, std::string>& headers = {}
    ) = 0;

    virtual std::optional<HttpResponse> put(
        const std::string& url,
        const std::string& body,
        const std::map<std::string, std::string>& headers = {}
    ) = 0;

};
