#pragma once

#include "Peer.h"
#include "SimulationKtp.h"

class Swarm //: public std::enable_shared_from_this<Swarm>
{
private:
    std::vector<Peer>                       m_Peers;
    Peer                                    m_bootstrapNode;
    bool                                    m_useTcp;
//    SimulationKtp                           m_protocol; // if useTcpMode = false

public:
    Swarm(bool mode, int PeerNumber)
        : m_useTcp(mode)
//        , m_protocol(std::shared_ptr<Swarm>(this))
    {
        generateSwarm(PeerNumber, mode);
    };

    void generateSwarm(size_t Peers, bool mode);
    void createBootstrapNode();
    bool tcp() { return m_useTcp; };
    std::vector<Peer> Peers() { return m_Peers; };
};

// tcp & offline
