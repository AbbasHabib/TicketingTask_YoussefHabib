#include "PahoMqttClient.hpp"
#include <iostream>
#include <memory>
#include <string>


bool PahoMqttClient::connect(const std::string& server_uri, const std::string& client_id)
{
    m_client = std::make_unique<mqtt::client>(server_uri, client_id);

    auto connOpts = mqtt::connect_options_builder()
        .keep_alive_interval(std::chrono::seconds(20))
        .clean_session(false)
        .automatic_reconnect(true)
        .finalize();

    std::cout << "Connecting..." << std::endl;
    while (true)
    {
        try 
        {
            m_client->connect(connOpts);
            break; 
        }
        catch (const mqtt::exception& exc)
        {
            std::cerr << "Broker offline, retrying in 5s..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    std::cout << "connected..." << std::endl;
    return true;
}

bool PahoMqttClient::is_connected()
{
    return m_client && m_client->is_connected();
}

bool PahoMqttClient::subscribe_to_topic(const std::string& topic,
        std::function<void(const std::string& topic, const std::string& payload)> callback)
{
    std::cout << "subscribing to topic: " << topic << '\n';

    try
    {
        m_topic_map[topic] = callback;
        m_client->subscribe(topic, 1);

        std::cout << "subscribed to topic: " << topic << '\n';
        return true;
    }
    catch (const mqtt::exception& exc)
    {
        std::cout << "MQTT subscribe error: " << exc.what() << std::endl;
        return false;
    }
}

void PahoMqttClient::run()
{
    bool was_disconnected = true;
    while (true)
    {
        if (was_disconnected && m_client->is_connected())
        {
            try
            {
                std::cout << "Re-connected! Re-subscribing to ensure topics are active..." << std::endl;
                for(auto& [topic, _] : m_topic_map)
                {
                    m_client->subscribe(topic, 1);
                }
                was_disconnected = false;
            }
            catch (const mqtt::exception& exc)
            {
                std::cout << "MQTT subscribe error: " << exc.what() << std::endl;
            }
        }

        mqtt::const_message_ptr msg;
        if (m_client->try_consume_message_for(&msg, std::chrono::seconds(1)))
        {
            if (msg)
            {
                std::cout << "Topic: " << msg->get_topic() << " | Payload: " << msg->to_string() << std::endl;
                if(auto it = m_topic_map.find(msg->get_topic()); it != m_topic_map.end())
                {
                    it->second(msg->get_topic(), msg->get_payload());
                }
            }
        }

        // Optional: Periodic health check or internal tasks
        if (!m_client->is_connected()) {
            was_disconnected = true;
            std::cout << "Waiting for auto-reconnect..." << std::endl;
        }
    }
}



PahoMqttClient::~PahoMqttClient() = default;
