#include "Gate.hpp"
#include <iostream>

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

    Gate gate(gate_id);
    gate.init();
    gate.run();
}