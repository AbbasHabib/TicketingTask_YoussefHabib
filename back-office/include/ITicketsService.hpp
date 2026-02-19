#pragma once

#include "Ticket.hpp"
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>

class ITicketsService
{
public:
    virtual std::pair<TicketErrorCode, std::string> create_ticket_base64(
        int validity_in_days,
        int line_number,
        int64_t request_date
    ) = 0;

    virtual std::pair<TicketErrorCode, nlohmann::json> validate_ticket_base64(const std::string& base64_ticket) = 0;

    virtual bool is_expired(int64_t creation_date, int64_t validity_in_days) = 0;
    virtual bool is_expired(const Ticket& ticket) = 0;

    virtual ~ITicketsService() = default;
};
