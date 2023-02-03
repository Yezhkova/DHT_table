#pragma once


class INodeStatistics
{
public:
    INodeStatistics();
    virtual ~INodeStatistics() = default;
    virtual void onFindNodeResponse(bool find, int packetNumber) = 0;
    virtual void onPacketReceived() = 0; // used for packet counting
    virtual void onPacketSent() = 0;

};
