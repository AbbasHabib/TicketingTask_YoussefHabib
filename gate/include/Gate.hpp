#pragma once


#include "GateId.hpp"
#include "IMqttClient.hpp"
#include "TicketProcessor.hpp"
#include <memory>

class Gate
{
public:
    Gate(GateId);

    bool init();
    void run();
private:
    GateId m_gate_id;
    std::shared_ptr<IMqttClient> m_mqtt_client;
    std::shared_ptr<IHttpClient> m_http_client;
    std::shared_ptr<TransactionTrackingClient> m_grpc_client;
    std::unique_ptr<TicketProcessor> m_ticket_processor;
};
