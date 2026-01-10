#pragma once


#include <cctype>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>



#include "IMqttClient.hpp"
#include "mqtt/client.h"


class PahoMqttClient : public IMqttClient
{
public:
    bool connect(const std::string& server_uri, const std::string& client_id) override;

    bool is_connected() override;

    bool subscribe_to_topic(const std::string& topic, std::function<void(const std::string&, const std::string&)> callback) override;

    void run() override;

    ~PahoMqttClient();

private:
    std::unique_ptr<mqtt::client> m_client;
    std::unordered_map<std::string, std::function<void(const std::string& topic, const std::string& payload)>> m_topic_map;

    void block_till_connected();

};





