#pragma once

#include <string>

class INewTicketObserver
{
public:
    virtual ~INewTicketObserver() = default;
    virtual void on_new_ticket(const std::string& ticket_base64) = 0;
};