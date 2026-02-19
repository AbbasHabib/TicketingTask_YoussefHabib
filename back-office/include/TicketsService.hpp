#pragma once
#include "ITicketsService.hpp"
#include "ITicketsRepository.hpp"
#include <atomic>
#include <string>

class TicketsService : public ITicketsService
{
public:
    explicit TicketsService(ITicketsRepository& repo);

    std::pair<TicketErrorCode, std::string> create_ticket_base64(
        int validity_in_days,
        int line_number,
        int64_t request_date
    ) override;

    std::pair<TicketErrorCode, nlohmann::json> validate_ticket_base64(const std::string& base64_ticket) override;

    bool is_expired(int64_t creation_date, int64_t validity_in_days) override;
    bool is_expired(const Ticket& ticket) override;

private:
    ITicketsRepository& m_repository;
    std::atomic<int64_t> m_id_counter{0};

    std::string base64_encode(const std::string& in);
    std::string base64_decode(const std::string& in);
};
