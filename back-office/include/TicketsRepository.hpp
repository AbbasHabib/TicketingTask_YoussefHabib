#pragma once
#include "Ticket.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <optional>
#include <string>

class TicketsRepository {
public:
    explicit TicketsRepository(const std::string& file);

    void save(const Ticket& ticket);
    std::optional<Ticket> find_by_id(int64_t ticket_id);
    std::vector<Ticket> find_all();
    int64_t get_last_ticket_id() const;

private:
    std::string file_path;

    nlohmann::json load();
    void persist(const nlohmann::json& data);
};
