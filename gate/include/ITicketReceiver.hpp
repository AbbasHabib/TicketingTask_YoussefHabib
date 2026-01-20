#pragma once

#include "INewTicketObserver.hpp"
#include <memory>

// Interface made simple only for the app usecases
class ITicketReceiver
{
public:
    virtual bool init() = 0;
    virtual void run() = 0;
    virtual void add_new_ticket_observer(std::shared_ptr<INewTicketObserver> observer) = 0;

    virtual ~ITicketReceiver() = default;
};

