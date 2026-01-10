#include "PahoMqttClient.hpp"
#include <iostream>


bool PahoMqttClient::connect(std::string_view server_uri, std::string_view client_id)
{
    try
    {
        static bool once = false;
        if(!once)
        {
            m_client = std::make_unique<mqtt::async_client>(std::string(server_uri), std::string(client_id));
            once = true;
        }

        mqtt::connect_options connOpts;
        connOpts.set_clean_session(false);
        connOpts.set_automatic_reconnect(true);
        
        m_client->connect(connOpts)->wait();

        m_client->set_message_callback([this](mqtt::const_message_ptr msg) {
            std::cout << "received from mqtt topic=" << msg->get_topic() << " payload= "<< msg->get_payload();
            auto it = m_topic_map.find(msg->get_topic());
            if (it != m_topic_map.end())
            {
                it->second(msg->get_topic(), msg->to_string());
            }
        });


        m_client->start_consuming();

        std::cout << "MQTT connected: " << server_uri << ", " << client_id << std::endl;

        return true;
    }
    catch (const mqtt::exception& exc)
    {
        std::cout << "MQTT connect error: " << exc.what() << std::endl;
        return false;
    }
}

bool PahoMqttClient::is_connected()
{
    return m_client && m_client->is_connected();
}

bool PahoMqttClient::subscribe_to_topic(const std::string& topic,
        std::function<void(const std::string&, const std::string&)> callback)
{
    std::cout << "subscribing to topic: " << topic << '\n';

    if (!m_client || !m_client->is_connected())
    {
        std::cout << "Not connected !! cant sub to " << topic << '\n';
        return false;
    }

    try
    {
        m_topic_map[topic] = callback;
        m_client->subscribe(topic, 0)->wait();
        
        std::cout << "subscribed to " << topic << '\n';
        return true;
    }
    catch (const mqtt::exception& exc)
    {
        std::cout << "MQTT subscribe error: " << exc.what() << std::endl;
        return false;
    }
}

PahoMqttClient::~PahoMqttClient()
{
    if (m_client && m_client->is_connected())
    {
        m_client->disconnect()->wait();
    }
}
