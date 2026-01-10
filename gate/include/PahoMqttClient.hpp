#pragma once


#include <cctype>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>


#include "IMqttClient.hpp"
#include "mqtt/async_client.h"


class PahoMqttClient : public IMqttClient
{
public:
    bool connect(std::string_view server_uri, std::string_view clientId) override;

    bool is_connected() override;

    bool subscribe_to_topic(const std::string& topic, std::function<void(const std::string&, const std::string&)> callback) override;

    ~PahoMqttClient();

private:
    std::unique_ptr<mqtt::async_client> m_client;
    std::unordered_map<std::string, std::function<void(const std::string&, const std::string&)>> m_topic_map;

    void block_till_connected();

};





