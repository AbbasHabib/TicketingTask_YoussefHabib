#include "Gate.hpp"
#include "TicketProcessor.hpp"
#include <iostream>
#include <memory>
#include <thread>


Gate::Gate(std::shared_ptr<ITicketReceiver> ticket_receiver,
    std::shared_ptr<TicketProcessor> ticket_processor)
    : m_ticket_receiver(ticket_receiver),
      m_ticket_processor(ticket_processor)
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
