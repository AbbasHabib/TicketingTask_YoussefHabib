#include "MqttTicketReceiver.hpp"

#include <iostream>

MqttTicketReceiver::MqttTicketReceiver(std::shared_ptr<IMqttClient> mqtt_client,
                                       GateId gate_id)
    : m_mqtt_client(mqtt_client),
      m_gate_id(gate_id)
{
}

bool MqttTicketReceiver::init()
{
    std::cout << "connecting to the MQTT broker\n";

    auto is_connected = m_mqtt_client->connect(MQTT_SERVER_URI, m_gate_id.str());

    std::cout << "Connected MQTT broker\n";

    std::string device_id = m_gate_id.str();

    m_mqtt_client->subscribe_to_topic(
        std::string("/transport/gate/") + device_id + "/event/validate",
        [this](const std::string& topic, const std::string& payload) {
            std::cout << "Received ticket request : topic " << topic
                      << " msg: " << payload << '\n';
            std::cout << "Notifying observers\n";

            notify_observers(payload);
        });

    return is_connected;
}

void MqttTicketReceiver::run()
{
    m_mqtt_client->run();
}

void MqttTicketReceiver::add_new_ticket_observer(std::shared_ptr<INewTicketObserver> observer)
{
    std::cout << "Adding new ticket observer\n";

    std::unique_lock<std::mutex> lk(m_observers_mtx);
    m_observers.insert(std::move(observer));
}

void MqttTicketReceiver::notify_observers(const std::string& ticket_base64)
{
    std::cout << "notifying new ticket observers\n";

    std::unique_lock<std::mutex> lk(m_observers_mtx);
    for (auto& observer : m_observers)
    {
        observer->on_new_ticket(ticket_base64);
    }
}
