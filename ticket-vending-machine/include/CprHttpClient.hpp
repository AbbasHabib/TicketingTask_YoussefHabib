#pragma once

#include "IHttpClient.hpp"
#include <cpr/cpr.h>

class CprHttpClient : public IHttpClient
{
public:
    std::optional<HttpResponse> get(
        const std::string& url,
        const std::string& body,
        const std::map<std::string, std::string>& headers = {}
    ) override;

    std::optional<HttpResponse> post(
        const std::string& url,
        const std::string& body,
        const std::map<std::string, std::string>& headers = {}
    ) override;

    std::optional<HttpResponse> put(
        const std::string& url,
        const std::string& body,
        const std::map<std::string, std::string>& headers = {}
    ) override;

};
