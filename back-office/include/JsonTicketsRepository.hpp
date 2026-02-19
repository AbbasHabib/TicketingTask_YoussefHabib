#pragma once
#include "ITicketsRepository.hpp"
#include "Ticket.hpp"
#include <mutex>
#include <nlohmann/json.hpp>
#include <vector>
#include <optional>
#include <string>

class JsonTicketsRepository : public ITicketsRepository
{
public:
    explicit JsonTicketsRepository(const std::string& file);

    void save(const Ticket& ticket) override;
    std::optional<Ticket> find_by_id(int64_t ticket_id) override;
    std::vector<Ticket> find_all() override;
    int64_t get_last_ticket_id() override;

    JsonTicketsRepository(const JsonTicketsRepository&) = delete;
    JsonTicketsRepository& operator=(const JsonTicketsRepository&) = delete;
    JsonTicketsRepository& operator=(JsonTicketsRepository&&) = delete;
    JsonTicketsRepository(JsonTicketsRepository&&) = delete;

private:
    nlohmann::json load();
    void persist(const nlohmann::json& data);
    
    std::string m_json_file_path;
    std::mutex m_mtx;
};
