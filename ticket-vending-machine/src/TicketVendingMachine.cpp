#include "TicketVendingMachine.hpp"
#include "PahoMqttClient.hpp"
#include "CprHttpClient.hpp"
#include "TicketProcessor.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>



// TicketVendingMachine::TicketVendingMachine(std::shared_ptr<IMqttClient> mqtt_client, std::unique_ptr<TicketProcessor> ticket_processor)
//     : m_mqtt_client(mqtt_client),
//       m_ticket_processor(std::move(ticket_processor))
// {}

TicketVendingMachine::TicketVendingMachine() :
    m_mqtt_client(std::make_shared<PahoMqttClient>()),
    m_http_client(std::make_shared<CprHttpClient>()),
    m_ticket_processor(std::make_unique<TicketProcessor>(m_mqtt_client, m_http_client))
{
}


bool TicketVendingMachine::init()
{
    std::cout << "connecting to the MQTT broker \n";

    m_mqtt_client->connect(MQTT_SERVER_URI, "TVM-1");

    std::cout << "Connected MQTT broker \n";
    
    if(!m_ticket_processor->init())
    {
        std::cout << "failed to init ticket processer \n";
        return false;
    }

    return true;
}

void TicketVendingMachine::run()
{
    m_ticket_processor->run();
}


