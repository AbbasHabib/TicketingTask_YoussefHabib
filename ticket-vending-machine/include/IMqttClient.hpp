#pragma once

#include <functional>
#include <string>

// Interface made simple only for the app usecases
class IMqttClient
{
public:
    virtual bool connect(const std::string& server_uri, const std::string& client_id) = 0;
    virtual bool is_connected() = 0;
    virtual bool subscribe_to_topic(const std::string& topic, std::function<void(const std::string& topic, const std::string& payload)> callback) = 0;
    virtual void run() = 0;
};