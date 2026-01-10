#include "CprHttpClient.hpp"
#include <iostream>
#include <optional>


std::optional<HttpResponse> CprHttpClient::get(
    const std::string& url,
    const std::string& body,
    const std::map<std::string, std::string>& headers)
{
    auto r = cpr::Get(
        cpr::Url{url},
        headers,
        cpr::Body{body},
        cpr::Timeout{3000} // 3 seconds
    );

    if (r.error)
    {
        // Log or handle the error
        std::cout << "[GET] HTTP request failed: " << r.error.message << "\n";
        return std::nullopt;
    }

    return HttpResponse{
        r.status_code,
        r.text,
        {r.header.begin(), r.header.end()}
    };
}

std::optional<HttpResponse> CprHttpClient::post(
    const std::string& url,
    const std::string& body,
    const std::map<std::string, std::string>& headers)
{
    auto r = cpr::Post(
        cpr::Url{url},
        headers,
        cpr::Body{body},
        cpr::Timeout{3000} // 3 seconds
    );

    if (r.error)
    {
        std::cout << "[POST] HTTP request failed: " << r.error.message << "\n";
        return std::nullopt;
    }

    return HttpResponse{
        r.status_code,
        r.text,
        {r.header.begin(), r.header.end()}
    };
}

std::optional<HttpResponse> CprHttpClient::put(
    const std::string& url,
    const std::string& body,
    const std::map<std::string, std::string>& headers)
{
    auto r = cpr::Put(
        cpr::Url{url},
        headers,
        cpr::Body{body},
        cpr::Timeout{3000} // 3 seconds
    );

    if (r.error)
    {
        std::cout << "[PUT] HTTP request failed: " << r.error.message << "\n";
        return std::nullopt;
    }

    return HttpResponse{
        r.status_code,
        r.text,
        {r.header.begin(), r.header.end()}
    };
}
