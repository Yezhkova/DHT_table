#pragma once


class INodeEventHandler
{
public:
    INodeEventHandler() {};
    virtual ~INodeEventHandler() = default;
    virtual void onFindNodeResponse(bool find) = 0;
    virtual void onPingResponse(bool find) = 0;
    virtual void onPacketReceived() = 0; // used for packet counting
    virtual void onPacketSent() = 0;
    virtual void onNodeNotFound() = 0;
};
