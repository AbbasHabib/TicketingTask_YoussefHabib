#pragma once

#include <memory>
#include <mutex>
#include <set>
#include <string>

#include "GateId.hpp"
#include "IMqttClient.hpp"
#include "INewTicketObserver.hpp"
#include "ITicketReceiver.hpp"

class MqttTicketReceiver : public ITicketReceiver
{
public:
    MqttTicketReceiver(std::shared_ptr<IMqttClient> mqtt_client, GateId gate_id);

    bool init() override;
    void run() override;

    void add_new_ticket_observer(std::shared_ptr<INewTicketObserver> observer) override;

private:
    void notify_observers(const std::string& ticket_base64);

private:
    std::shared_ptr<IMqttClient> m_mqtt_client;
    GateId m_gate_id;

    std::mutex m_observers_mtx;
    std::set<std::shared_ptr<INewTicketObserver>> m_observers;
};
