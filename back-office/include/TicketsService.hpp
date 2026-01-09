#pragma once
#include "TicketsRepository.hpp"
#include <atomic>
#include <string>

class TicketsService {
public:
    explicit TicketsService(TicketsRepository& repo);

    std::pair<TicketErrorCode, std::string> create_ticket_base64(
        int validity_days,
        int line_number,
        int64_t request_date
    );

    std::pair<TicketErrorCode, nlohmann::json> validate_ticket_base64(const std::string& base64_ticket);

private:
    TicketsRepository& repository;
    std::atomic<int64_t> id_counter{0};

    std::string base64_encode(const std::string& in);
    std::string base64_decode(const std::string& in);

    bool is_expired(const Ticket& ticket);
};
