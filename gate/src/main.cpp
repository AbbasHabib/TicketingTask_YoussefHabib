#include "CprHttpClient.hpp"
#include "GateId.hpp"
#include "Gate.hpp"
#include "MqttTicketReceiver.hpp"
#include "PahoMqttClient.hpp"
#include "TransactionTrackingClient.hpp"
#include <iostream>
#include <boost/di.hpp>


GateId get_env_gate_id()
{
    const char* gate_id = std::getenv(GATE_ID_ENV);

    if (gate_id != nullptr)
    {
        std::cout << "The value of gate_id is: " << gate_id << std::endl;
        return GateId(gate_id);
    }
    else
    {
        return GateId("GATE-1");
        std::cout << "The environment variable PATH was not found." << std::endl;
    }
}


int main()
{   
    GateId gate_id = get_env_gate_id();

    std::cout << "[GATE] Started with GateID= " << gate_id.str() << '\n';
    
    namespace di = boost::di;

    auto injector = di::make_injector(
        di::bind<GateId>.to(gate_id),
        di::bind<IMqttClient>.to<PahoMqttClient>().in(di::singleton),
        di::bind<IHttpClient>.to<CprHttpClient>().in(di::singleton),
        di::bind<ITransactionTrackingClient>.to<TransactionTrackingClient>().in(di::singleton),
        di::bind<ITicketReceiver>.to<MqttTicketReceiver>().in(di::singleton),
        di::bind<TicketProcessor>.in(di::singleton)
    );

    auto gate = injector.create<std::shared_ptr<Gate>>();
    if(gate->init())
    {
        gate->run();
    }
    else
    {
        std::cerr << "[GATE] Failed to init the gate GateID=" << gate_id.str() << '\n';
    }
}