#pragma once
#define FAST_EMULATION

struct NodeRef
{
	//constructors
//#ifdef FAST_EMULATION
//	Peer* m_peer;
//#else
//	const ID* m_nodeId;
//#endif

	const ID& getId() {
		// in .cpp
	}
	Peer* getPeer() {
		return m_peer;
	}
private:
#ifdef FAST_EMULATION
	Peer* m_peer;
#else
	const ID* m_nodeId;
#endif

};

