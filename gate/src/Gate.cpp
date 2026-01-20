#include "Gate.hpp"
#include "GateId.hpp"
#include "MqttTicketReceiver.hpp"
#include "PahoMqttClient.hpp"
#include "CprHttpClient.hpp"
#include "TicketProcessor.hpp"
#include "TransactionTrackingClient.hpp"
#include <iostream>
#include <memory>
#include <thread>


Gate::Gate(GateId gate_id) :
    m_gate_id(gate_id),
    m_mqtt_client(std::make_shared<PahoMqttClient>()),
    m_http_client(std::make_shared<CprHttpClient>()),
    m_grpc_client(std::make_shared<TransactionTrackingClient>(GRPC_SERVER_URI)),
    m_ticket_receiver(std::make_shared<MqttTicketReceiver>(m_mqtt_client, gate_id)),
    m_ticket_processor(std::make_shared<TicketProcessor>(m_ticket_receiver, m_http_client, m_grpc_client, gate_id))
{
}


bool Gate::init()
{
    if(!m_ticket_receiver->init())
    {
        std::cout << "failed to init ticket processer \n";
        return false;
    }

    if(!m_ticket_processor->init())
    {
        std::cout << "failed to init ticket processer \n";
        return false;
    }

    return true;
}

void Gate::run()
{
    std::jthread jt([this]{ m_ticket_receiver->run(); });
    m_ticket_processor->run();
}


