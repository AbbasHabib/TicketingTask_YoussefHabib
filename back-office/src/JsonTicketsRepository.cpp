#include "JsonTicketsRepository.hpp"
#include <fstream>
#include <mutex>
#include <algorithm>

using json = nlohmann::json;

JsonTicketsRepository::JsonTicketsRepository(const std::string& file)
    : m_json_file_path(file) {}

json JsonTicketsRepository::load()
{
    std::ifstream f(m_json_file_path);
    if (!f.is_open())
    {
        return json{{"tickets", json::array()}};
    }
    json j;
    f >> j;
    if (j.is_discarded()) // handle malformed JSON
    {
        return json{{"tickets", json::array()}};
    }
    return j;
}

void JsonTicketsRepository::persist(const json& data)
{
    std::ofstream f(m_json_file_path);
    f << data.dump(4);
}

void JsonTicketsRepository::save(const Ticket& ticket)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto data = load();
    data["tickets"].push_back({
        {"ticket_id", ticket.ticket_id},
        {"creation_date", ticket.creation_date},
        {"validity_in_days", ticket.validity_in_days},
        {"line_number", ticket.line_number}
    });
    persist(data);
}

std::optional<Ticket> JsonTicketsRepository::find_by_id(int64_t id)
{
    std::lock_guard<std::mutex> lock(m_mtx);
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

std::vector<Ticket> JsonTicketsRepository::find_all()
{
    std::lock_guard<std::mutex> lock(m_mtx);
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

int64_t JsonTicketsRepository::get_last_ticket_id()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    auto data = load();
    int64_t max_id = 0;

    for (const auto& t : data["tickets"]) {
        max_id = std::max(max_id, t["ticket_id"].get<int64_t>());
    }
    return max_id;
}
