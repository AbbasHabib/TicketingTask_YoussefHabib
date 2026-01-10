#pragma once

#include "GenericRetryQueue.hpp"
#include "IHttpClient.hpp"
#include "IMqttClient.hpp"
#include "Ticket.hpp"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include "GateId.hpp"
#include "TransactionTrackingClient.hpp"


class TicketProcessor
{
public:
    TicketProcessor(std::shared_ptr<IMqttClient> mqtt_client, std::shared_ptr<IHttpClient> m_http_client, std::shared_ptr<TransactionTrackingClient> transaction_report_client, GateId gate_id);

    bool init();
    void run();

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
    void on_new_ticket(const std::string& topic, const std::string& payload);
    bool parse_ticket(const nlohmann::json& body, Ticket& t);
    bool parse_base64_ticket(const std::string& body, Ticket& t);
    void build_and_persist_xml();
    bool persist_xml_report(const std::string& xml);
    void record_transaction(const TicketValidation& ticket_validation);
    bool is_ticket_expired(const Ticket& ticket);
    bool sendValidationStatsToServer(const ValidationStats& validation_stat);




    std::shared_ptr<IMqttClient> m_mqtt_client;
    std::shared_ptr<IHttpClient> m_http_client;
    ValidationStats m_stats;
    std::vector<TicketValidation> m_last_validations;
    std::shared_ptr<TransactionTrackingClient> m_transaction_report_client;
    GateId m_gate_id;
    GenericRetryQueue<ValidationStats> m_retryQ;
};

