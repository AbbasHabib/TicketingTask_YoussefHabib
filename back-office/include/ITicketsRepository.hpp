#pragma once
#include "Ticket.hpp"
#include <vector>
#include <optional>

class ITicketsRepository {
public:
    virtual void save(const Ticket& ticket) = 0;
    virtual std::optional<Ticket> find_by_id(int64_t ticket_id) = 0;
    virtual std::vector<Ticket> find_all() = 0;
    virtual int64_t get_last_ticket_id() = 0;

    virtual ~ITicketsRepository() = default;
};
