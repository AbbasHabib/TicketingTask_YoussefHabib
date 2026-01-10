#pragma once


#include "IMqttClient.hpp"
#include "TicketProcessor.hpp"
#include <memory>
class TicketVendingMachine
{
public:
    // TicketVendingMachine(std::shared_ptr<IMqttClient> mqtt_client, std::unique_ptr<TicketProcessor> ticket_processor);
    TicketVendingMachine();

    bool init();
    void run();
private:
    std::shared_ptr<IMqttClient> m_mqtt_client;
    std::shared_ptr<IHttpClient> m_http_client;
    std::unique_ptr<TicketProcessor> m_ticket_processor;
};
