#include "TicketProcessor.hpp"
#include "IHttpClient.hpp"
#include "TicketRequest.hpp"
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>

TicketProcessor::TicketProcessor(std::shared_ptr<IMqttClient> mqtt_client, std::shared_ptr<IHttpClient> http_client) 
    : m_mqtt_client(mqtt_client),
      m_http_client(http_client)
{}

bool TicketProcessor::init()
{
    // TODO: it's only 1 instance for now make it configurable
    m_mqtt_client->subscribe_to_topic(std::string("/transport/tvm/")+"1"+"/event/create", 
        [this](const std::string& topic, const std::string& payload){ 
            on_new_ticket(topic, payload); 
        });
    return true;
}

void TicketProcessor::run()
{
    m_mqtt_client->run();
}

// TODO: to be hanled by a receiver class
void TicketProcessor::on_new_ticket(const std::string& topic, const std::string& payload)
{
    using json = nlohmann::json;

    std::cout << "Received ticket request : topic" << topic << " msg: " << payload;


    auto body = json::parse(payload, nullptr, false);
    if (body.is_discarded())
    {
        std::cout << "invalid ticket request received";
    }

    TicketRequest t;
    if(!parse_ticket(body, t))
    {
        std::cout << "ticket json is invalid\n";
        return;
    }

    std::cout << "sending \n";
    auto response = m_http_client->post(HTTP_SERVER_URI"/api/v1/tickets", payload);

    if(!response.has_value())
    {
        std::cout << "Failed to send GET request \n";
        // handle ticket validation failure offline
    }
    else
    {
        if(response->status_code >= 200 && response->status_code < 300)
        {
            std::cout << "Successfully sent ticket request to the RESTapi response: " << static_cast<std::string>(*response) << '\n';
            // handle ticket validation OK
        }
        else
        {
            std::cout << "Bad Request to the RESTapi response: " << static_cast<std::string>(*response) << '\n';
            // handle ticket validation failure XML
        }
    }
}

bool TicketProcessor::parse_ticket(const nlohmann::json& body, TicketRequest& t)
{
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

    if (body.contains("request_date") && body["request_date"].is_number_integer())
    {
        t.request_date = body["request_date"];
    }
    else
    {
        std::cout << "Missing or invalid 'request_date'\n";
        valid = false;
    }

    return valid;
}
