#pragma once

#include "IHttpClient.hpp"
#include "IMqttClient.hpp"
#include "TicketRequest.hpp"
#include <memory>
#include <nlohmann/json_fwd.hpp>

class TicketProcessor
{
public:
    TicketProcessor(std::shared_ptr<IMqttClient> mqtt_client, std::shared_ptr<IHttpClient> m_http_client);

    bool init();
    void run();

private:
    void on_new_ticket(const std::string& topic, const std::string& payload);
    bool parse_ticket(const nlohmann::json& body, TicketRequest& t);

    std::shared_ptr<IMqttClient> m_mqtt_client;
    std::shared_ptr<IHttpClient> m_http_client;
};

