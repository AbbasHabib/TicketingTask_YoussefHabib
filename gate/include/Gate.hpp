#pragma once


#include "GateId.hpp"
#include "IMqttClient.hpp"
#include "ITicketReceiver.hpp"
#include "TicketProcessor.hpp"
#include <memory>

class Gate
{
public:
    Gate(std::shared_ptr<ITicketReceiver> ticket_receiver,
         std::shared_ptr<TicketProcessor> ticket_processor);

    bool init();
    void run();
private:
    std::shared_ptr<ITicketReceiver> m_ticket_receiver;
    std::shared_ptr<TicketProcessor> m_ticket_processor;
};
