#include "Gate.hpp"
#include "GateId.hpp"
#include "PahoMqttClient.hpp"
#include "CprHttpClient.hpp"
#include "TicketProcessor.hpp"
#include <iostream>
#include <memory>


Gate::Gate(GateId gate_id) :
    m_gate_id(gate_id),
    m_mqtt_client(std::make_shared<PahoMqttClient>()),
    m_http_client(std::make_shared<CprHttpClient>()),
    m_ticket_processor(std::make_unique<TicketProcessor>(m_mqtt_client, m_http_client, gate_id))
{
}


bool Gate::init()
{
    std::cout << "connecting to the MQTT broker \n";

    m_mqtt_client->connect(MQTT_SERVER_URI, m_gate_id.str());

    std::cout << "Connected MQTT broker \n";
    
    if(!m_ticket_processor->init())
    {
        std::cout << "failed to init ticket processer \n";
        return false;
    }

    return true;
}

void Gate::run()
{
    m_ticket_processor->run();
}


