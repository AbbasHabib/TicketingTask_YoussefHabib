#include "TicketsService.hpp"
#include "Ticket.hpp"
#include <crow/logging.h>
#include <crow/utility.h>
#include <cstdint>
#include <ctime>
#include <iostream>

using json = nlohmann::json;

TicketsService::TicketsService(TicketsRepository& repo)
    : m_repository(repo) {}

std::pair<TicketErrorCode, std::string> TicketsService::create_ticket_base64(int validity_in_days, int line_number, int64_t request_date)
{
    // check validity of the ticket before persisting
    if (is_expired(request_date, validity_in_days))
    {
        return {TicketErrorCode::Expired, "ticket in the request is expired!"};
    }

    // Ticket is from future
    int64_t now = std::time(nullptr);
    if (request_date > now)
    {
        return {TicketErrorCode::FromFuture, "ticket has date from future"};
    }

    Ticket ticket{
        m_repository.get_last_ticket_id() + 1, // TODO: very slow, + could face bad racing 
        request_date, // NOTE: As in the readme we are assuming that the creation date is the same as request date
        validity_in_days,
        line_number
    };


    // TODO: Ticket is highly delayed

    m_repository.save(ticket);

    json ticket_json = {
        {"ticket_id", ticket.ticket_id},
        {"creation_date", ticket.creation_date},
        {"validity_in_days", ticket.validity_in_days},
        {"line_number", ticket.line_number}
    };

    return {TicketErrorCode::NoErr, base64_encode(ticket_json.dump())};
}

std::pair<TicketErrorCode, nlohmann::json> TicketsService::validate_ticket_base64(const std::string& encoded)
{
    // {"creation_date":1768071208,"line_number":2,"ticket_id":9,"validity_in_days":999}

    CROW_LOG_INFO << " encoded msg: [ " << encoded << ']' << '\n';

    auto decoded = base64_decode(encoded);

    CROW_LOG_INFO << " decoded msg: [ " << decoded << ']' << '\n';
    
    auto j = json::parse(decoded, nullptr, false);
    if (j.is_discarded())
    {
        return {TicketErrorCode::InvalidPayload, {
            {"valid", false},
            {"reason", "INVALID_PAYLOAD"}
            }
        };
    }

    int64_t ticket_id = j["ticket_id"];
    auto ticket = m_repository.find_by_id(ticket_id);

    if (!ticket.has_value())
    {
        return {TicketErrorCode::NotFound, {
            {"valid", false},
            {"reason", "NOT_FOUND"}
            }
        };
    }


    if (is_expired(*ticket))
    {
        return {TicketErrorCode::Expired, {
            {"valid", false},
            {"reason", "EXPIRED"}
            }
        };
    }

    return {TicketErrorCode::NoErr,{
        {"ticket_id", ticket_id},
        {"valid", true},
        {"reason", "VALIDATED"}
        }
    };
}

bool TicketsService::is_expired(const Ticket& t)
{
    int64_t now = std::time(nullptr);
    int64_t expiry = t.creation_date + (t.validity_in_days * 86400); // 86400= 24 * 60 * 60
    return now > expiry;
}

bool TicketsService::is_expired(int64_t creation_date, int64_t validity_in_days)
{
    int64_t now = std::time(nullptr);
    int64_t expiry = creation_date + (validity_in_days * 86400); // 86400= 24 * 60 * 60
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
