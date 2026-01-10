#pragma once

#include <functional>
#include <string_view>

// Interface made simple only for the app usecases
class IMqttClient
{
public:
    virtual bool connect(std::string_view server_uri, std::string_view clientId) = 0;
    virtual bool is_connected() = 0;
    virtual bool subscribe_to_topic(const std::string& topic, std::function<void(const std::string& topic, const std::string& payload)> callback) = 0;
};