#include "SimulationKtp.h"

void SimulationKtp::ping(const ID & id)
{
    LOG("Node " << std::string(id) << " is online\n");
}
