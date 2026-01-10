#include "TicketProcessor.hpp"
#include "IHttpClient.hpp"
#include "Ticket.hpp"
#include <chrono>
#include <crow/utility.h>
#include <functional>
#include <iostream>
#include <tinyxml2.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>

TicketProcessor::TicketProcessor(std::shared_ptr<IMqttClient> mqtt_client, std::shared_ptr<IHttpClient> http_client, GateId gate_id) 
    : m_mqtt_client(mqtt_client),
      m_http_client(http_client),
      m_gate_id(gate_id)
{}

bool TicketProcessor::init()
{
    std::string device_id = m_gate_id.str();
    // TODO: it's only 1 instance for now make it configurable
    m_mqtt_client->subscribe_to_topic(std::string("/transport/gate/")+device_id+"/event/validate", 
        [this](const std::string& topic, const std::string& payload){ 
            on_new_ticket(topic, payload); 
        });
    return true;
}

void TicketProcessor::run()
{
    m_mqtt_client->run();
}

void TicketProcessor::record_transaction(const TicketValidation& ticket_validation)
{
    bool is_online = false;
    bool is_valid = false;

    m_stats.total++;

    if(ticket_validation.strategy == ValidationStrategy::ONLINE)
    {
        is_online = true;
        m_stats.online++;
    }
    else
    {
        m_stats.offline++;
    }

    if(ticket_validation.result == ValidationResult::VALID)
    {
        is_valid = true;
        m_stats.valid++;
    }
    else
    {
        m_stats.invalid++;
    }
    
    std::cout << "New Transaction Recored: " 
            << (is_valid ? "Valid Transaction " : "Invalid Transaction ")
            << (is_online ? "Online validated " : "Offline Validated " ) << '\n';

    m_last_validations.push_back(ticket_validation);
    build_and_persist_xml();
}


// TODO: to be hanled by a receiver class
void TicketProcessor::on_new_ticket(const std::string& topic, const std::string& payload)
{
    using json = nlohmann::json;

    std::cout << "Received ticket request : topic" << topic << " msg: " << payload;
    
    // invalid requst before sending to the wire
    Ticket t;
    if(!parse_base64_ticket(payload, t))
    {
        ValidationStrategy strategy = ValidationStrategy::OFFLINE;
        ValidationResult validation_result = ValidationResult::INVALID;
        record_transaction({.ticket_base64=payload, .strategy=strategy, .result=validation_result, .timestamp=std::time(nullptr)});
        return;
    }

    std::cout << "sending \n";
    auto response = m_http_client->get(HTTP_SERVER_URI"/api/v1/tickets/validations", payload);

    // case wasn't able to send to remote
    if(!response.has_value())
    {
        std::cout << "Failed to send GET request \n";
        
        std::cout << "validating ticket offline \n";
        // handle ticket validation failure offline
        if(is_ticket_expired(t))
        {
            ValidationStrategy strategy = ValidationStrategy::OFFLINE;
            ValidationResult validation_result = ValidationResult::INVALID;
            record_transaction({.ticket_base64=payload, .strategy=strategy, .result=validation_result, .timestamp=std::time(nullptr)});
        }
        else
        {
            std::cout << "Ticket is not Expired \n";
            ValidationStrategy strategy = ValidationStrategy::OFFLINE;
            ValidationResult validation_result = ValidationResult::VALID;
            record_transaction({.ticket_base64=payload, .strategy=strategy, .result=validation_result, .timestamp=std::time(nullptr)});
        }
    }
    else
    {
        if(response->status_code >= 200 && response->status_code < 300)
        {
            std::cout << "Successfully sent ticket request to the RESTapi response: " << static_cast<std::string>(*response);

            ValidationStrategy strategy = ValidationStrategy::ONLINE;
            ValidationResult validation_result = ValidationResult::VALID;
            record_transaction({.ticket_base64=payload, .strategy=strategy,.result=validation_result,.timestamp= std::time(nullptr)});
        }
        else
        {
            std::cout << "Bad Request to the RESTapi response: " << static_cast<std::string>(*response);
            
            ValidationStrategy strategy = ValidationStrategy::ONLINE;
            ValidationResult validation_result = ValidationResult::INVALID;
            record_transaction({.ticket_base64=payload, .strategy=strategy,.result=validation_result,.timestamp= std::time(nullptr)});
        }
    }
}


bool TicketProcessor::is_ticket_expired(const Ticket& t)
{
    int64_t now = std::time(nullptr);
    int64_t expiry = t.creation_date + (t.validity_in_days * 86400); // 86400= 24 * 60 * 60
    return now > expiry;
}

bool TicketProcessor::parse_base64_ticket(const std::string& body, Ticket& t)
{
    using json = nlohmann::json;
    std::string decoded_str = crow::utility::base64decode(body.c_str(), body.size());
    json j = json::parse(decoded_str, nullptr, false);

    if (j.is_discarded())
    {
        std::cout << "Invalid base64 Ticket : Invalid JSON string\n";
        return false;
    }

    return parse_ticket(j, t);
}

bool TicketProcessor::parse_ticket(const nlohmann::json& body, Ticket& t)
{
    // example ticket {"validity_in_days": 1,"line_number": 1,"creation_date": 1767897089, "ticket_id":77}

    bool valid = true;

    if (body.contains("validity_in_days") && body["validity_in_days"].is_number_integer())
    {
        t.validity_in_days = body["validity_in_days"];
    }
    else
    {
        std::cout << "Missing or invalid 'validity_in_days'\n";
        valid = false;
    }

    if (body.contains("line_number") && body["line_number"].is_number_integer())
    {
        t.line_number = body["line_number"];
    }
    else
    {
        std::cout << "Missing or invalid 'line_number'\n";
        valid = false;
    }

    if (body.contains("creation_date") && body["creation_date"].is_number_integer())
    {
        t.creation_date = body["creation_date"];
    }
    else
    {
        std::cout << "Missing or invalid 'creation_date'\n";
        valid = false;
    }

    if (body.contains("ticket_id") && body["ticket_id"].is_number_integer())
    {
        t.ticket_id = body["ticket_id"];
    }
    else
    {
        std::cout << "Missing or invalid 'ticket_id'\n";
        valid = false;
    }

    return valid;
}



bool TicketProcessor::persist_xml_report(const std::string& xml)
{
    namespace fs = std::filesystem;

    auto ts = std::time(nullptr);
    std::string tmp = "/data/gate" + m_gate_id.str() + ".xml.tmp";
    std::string final = "/data/gate" + m_gate_id.str() + ".xml";

    {
        std::ofstream ofs(tmp, std::ios::out | std::ios::trunc);
        if (!ofs) return false;
        ofs << xml;
        ofs.flush();
    }

    fs::rename(tmp, final); // atomic
    return true;
}


void TicketProcessor::build_and_persist_xml()
{
    using namespace tinyxml2;

    XMLDocument doc;

    // Root
    auto* root = doc.NewElement("GateReport");
    doc.InsertFirstChild(root);

    // Gate ID
    {
        auto* e = doc.NewElement("GateId");
        e->SetText(m_gate_id.str().c_str());
        root->InsertEndChild(e);
    }

    // Summary
    auto* summary = doc.NewElement("Summary");
    root->InsertEndChild(summary);

    {
        auto* e = doc.NewElement("TotalValidations");
        e->SetText(m_stats.total);
        summary->InsertEndChild(e);
    }

    {
        auto* e = doc.NewElement("ValidCount");
        e->SetText(m_stats.valid);
        summary->InsertEndChild(e);
    }

    {
        auto* e = doc.NewElement("InvalidCount");
        e->SetText(m_stats.invalid);
        summary->InsertEndChild(e);
    }

    {
        auto* e = doc.NewElement("OnlineCount");
        e->SetText(m_stats.online);
        summary->InsertEndChild(e);
    }

    {
        auto* e = doc.NewElement("OfflineCount");
        e->SetText(m_stats.offline);
        summary->InsertEndChild(e);
    }

    // Validations
    auto* validations = doc.NewElement("Validations");
    root->InsertEndChild(validations);

    for (const auto& v : m_last_validations)
    {
        auto* validation = doc.NewElement("Validation");

        {
            auto* e = doc.NewElement("TicketBase64");
            e->SetText(v.ticket_base64.c_str());
            validation->InsertEndChild(e);
        }

        {
            auto* e = doc.NewElement("Strategy");
            e->SetText(
                v.strategy == ValidationStrategy::ONLINE ? "ONLINE" : "OFFLINE");
            validation->InsertEndChild(e);
        }

        {
            auto* e = doc.NewElement("Result");
            e->SetText(
                v.result == ValidationResult::VALID ? "VALID" : "INVALID");
            validation->InsertEndChild(e);
        }

        {
            auto* e = doc.NewElement("Timestamp");
            e->SetText(static_cast<uint64_t>(v.timestamp));
            validation->InsertEndChild(e);
        }

        validations->InsertEndChild(validation);
    }

    // Persist logic
    XMLPrinter printer;
    doc.Print(&printer);

    persist_xml_report(printer.CStr());
}

