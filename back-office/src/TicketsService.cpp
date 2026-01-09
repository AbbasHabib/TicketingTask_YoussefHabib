#include "TicketsService.hpp"
#include <crow/utility.h>
#include <ctime>

using json = nlohmann::json;

TicketsService::TicketsService(TicketsRepository& repo)
    : repository(repo) {}

std::string TicketsService::create_ticket_base64(int validity_days, int line_number, int64_t request_date)
{
    Ticket ticket{
        id_counter.fetch_add(1), // not correct 
        request_date,
        validity_days,
        line_number
    };

    repository.save(ticket);

    json j = {
        {"ticket_id", ticket.ticket_id},
        {"creation_date", ticket.creation_date},
        {"validity_in_days", ticket.validity_in_days},
        {"line_number", ticket.line_number}
    };

    return base64_encode(j.dump());
}

json TicketsService::validate_ticket_base64(const std::string& encoded)
{
    auto decoded = base64_decode(encoded);
    auto j = json::parse(decoded);

    int64_t ticket_id = j["ticket_id"];
    auto ticket = repository.find_by_id(ticket_id);

    if (!ticket.has_value())
    {
        return
        {
            {"valid", false},
            {"reason", "NOT_FOUND"}
        };
    }

    int64_t now = std::time(nullptr);
    if (is_expired(*ticket, now))
    {
        return
        {
            {"ticket_id", ticket_id},
            {"valid", false},
            {"reason", "EXPIRED"}
        };
    }

    return
    {
        {"ticket_id", ticket_id},
        {"valid", true},
        {"reason", "VALIDATED"}
    };
}

bool TicketsService::is_expired(const Ticket& t, int64_t now)
{
    int64_t expiry = t.creation_date + (t.validity_in_days * 86400);
    return now > expiry;
}


std::string TicketsService::base64_encode(const std::string& in)
{
    return crow::utility::base64encode(in.c_str(), in.size());
}


std::string TicketsService::base64_decode(const std::string& in)
{
    return crow::utility::base64decode(in.c_str(), in.size());
}
