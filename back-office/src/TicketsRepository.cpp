#include "TicketsRepository.hpp"
#include <fstream>

using json = nlohmann::json;

TicketsRepository::TicketsRepository(const std::string& file)
    : file_path(file) {}

json TicketsRepository::load()
{
    std::ifstream f(file_path);
    if (!f.is_open())
    {
        return json{{"tickets", json::array()}};
    }
    json j;
    f >> j;
    return j;
}

void TicketsRepository::persist(const json& data)
{
    std::ofstream f(file_path);
    f << data.dump(4);
}

void TicketsRepository::save(const Ticket& ticket)
{
    auto data = load();
    
    data["tickets"].push_back({
        {"ticket_id", ticket.ticket_id},
        {"creation_date", ticket.creation_date},
        {"validity_in_days", ticket.validity_in_days},
        {"line_number", ticket.line_number}
    });
    persist(data);
}

std::optional<Ticket> TicketsRepository::find_by_id(int64_t id)
{
    auto data = load();
    for (const auto& t : data["tickets"])
    {
        if (t["ticket_id"] == id)
        {
            return Ticket{
                t["ticket_id"],
                t["creation_date"],
                t["validity_in_days"],
                t["line_number"]
            };
        }
    }
    return std::nullopt;
}

std::vector<Ticket> TicketsRepository::find_all()
{
    std::vector<Ticket> result;
    auto data = load();
    for (const auto& t : data["tickets"])
    {
        result.push_back({
            t["ticket_id"],
            t["creation_date"],
            t["validity_in_days"],
            t["line_number"]
        });
    }
    return result;
}

int64_t TicketsRepository::get_last_ticket_id() const
{
    auto data = load();
    int64_t max_id = 0;

    for (const auto& t : data["tickets"]) {
        max_id = std::max(max_id, t["ticket_id"].get<int64_t>());
    }
    return max_id;
}

