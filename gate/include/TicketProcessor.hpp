#pragma once

#include "GenericRetryQueue.hpp"
#include "IHttpClient.hpp"
#include "INewTicketObserver.hpp"
#include "ITicketReceiver.hpp"
#include "Ticket.hpp"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include "GateId.hpp"
#include "ITransactionTrackingClient.hpp"


class TicketProcessor : public INewTicketObserver, public std::enable_shared_from_this<TicketProcessor> 
{
public:
    TicketProcessor(std::shared_ptr<ITicketReceiver> ticket_receiver, std::shared_ptr<IHttpClient> http_client, std::shared_ptr<ITransactionTrackingClient> transaction_report_client, GateId gate_id);

    bool init();
    void run();
    void on_new_ticket(const std::string& ticket_base64) override;

private:
    enum class ValidationStrategy
    {
        ONLINE,
        OFFLINE
    };

    enum class ValidationResult
    {
        VALID,
        INVALID
    };

    struct TicketValidation
    {
        std::string ticket_base64;
        ValidationStrategy strategy;
        ValidationResult result;
        int64_t timestamp;
    };


    struct ValidationStats
    {
        uint32_t total{0};
        uint32_t valid{0};
        uint32_t invalid{0};
        uint32_t online{0};
        uint32_t offline{0};
    };

private:
    bool parse_ticket(const nlohmann::json& body, Ticket& t);
    bool parse_base64_ticket(const std::string& body, Ticket& t);
    void build_and_persist_xml();
    bool persist_xml_report(const std::string& xml);
    void record_transaction(const TicketValidation& ticket_validation);
    bool is_ticket_expired(const Ticket& ticket);
    bool send_validation_stats_to_server(const ValidationStats& validation_stat);


    std::shared_ptr<ITicketReceiver> m_ticket_receiver_client;
    std::shared_ptr<IHttpClient> m_http_client;
    std::shared_ptr<ITransactionTrackingClient> m_transaction_report_client;
    GateId m_gate_id;

    ValidationStats m_stats;
    std::vector<TicketValidation> m_last_validations;
    GenericRetryQueue<ValidationStats> m_validation_stats_retryQ;
};

