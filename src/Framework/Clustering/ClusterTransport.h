#ifndef CLUSTERTRANSPORT_H
#define CLUSTERTRANSPORT_H

#include "System/Containers/List.h"
#include "Framework/Messaging/Message.h"
#include "ClusterServer.h"

#define UNDEFINED_NODEID    ((uint64_t)(-1))

class WriteReadyness;

/*
===============================================================================================

 ClusterTransport

===============================================================================================
*/

class ClusterTransport
{
public:
    virtual ~ClusterTransport();
    
    void                        Init(Endpoint& endpoint);
    
    void                        SetSelfNodeID(uint64_t nodeID);
    void                        SetClusterID(uint64_t clusterID);
    
    bool                        IsAwaitingNodeID();
    uint64_t                    GetSelfNodeID();
    Endpoint&                   GetSelfEndpoint();
    uint64_t                    GetClusterID();

    unsigned                    GetNumConns();
    unsigned                    GetNumWriteReadyness();

    void                        AddConnection(uint64_t nodeID, Endpoint& endpoint);
    bool                        HasConnection(uint64_t nodeID);
    bool                        IsConnected(uint64_t nodeID);
    Endpoint&                   GetEndpoint(uint64_t nodeID);
    bool                        SetConnectionNodeID(Endpoint& endpoint, uint64_t nodeID);
    
    void                        SendMessage(uint64_t nodeID, Buffer& prefix, Message& msg);
    
    void                        DropConnection(uint64_t nodeID);
    void                        DropConnection(Endpoint endpoint);
    
    virtual void                OnConnectionReady(uint64_t nodeID, Endpoint endpoint)           = 0;
    virtual void                OnConnectionEnd(uint64_t nodeID, Endpoint endpoint)             = 0;
    virtual bool                OnAwaitingNodeID(Endpoint endpoint)                             = 0;
    virtual void                OnMessage(uint64_t nodeID, ReadBuffer msg)                      = 0;

    bool                        GetNextWaiting(Endpoint& endpoint);
    
    void                        RegisterWriteReadyness(WriteReadyness* wr);
    void                        UnregisterWriteReadyness(WriteReadyness* wr);

private:
    // for ClusterConnection:
    void                        AddConnection(ClusterConnection* conn);
    ClusterConnection*          GetConnection(uint64_t nodeID);
    ClusterConnection*          GetConnection(Endpoint& endpoint);
    void                        DeleteConnection(ClusterConnection* conn);
    void                        ReconnectAll();
    void                        OnWriteReadyness(ClusterConnection* conn);

    bool                        awaitingNodeID;
    uint64_t                    nodeID;
    uint64_t                    clusterID;
    Endpoint                    endpoint;
    Buffer                      msgBuffer;
    ClusterServer               server;
    InList<ClusterConnection>   conns;
    InList<WriteReadyness>      writeReadynessList;

    friend class ClusterConnection;
};

/*
===============================================================================================

 WriteReadyness

===============================================================================================
*/

class WriteReadyness
{
public:
    WriteReadyness()
    {
        prev = next = this;
    }

    void SetCallable(Callable callable_)
    {
        callable = callable_;
    }

    uint64_t            nodeID;
    Callable            callable;
    WriteReadyness*     prev;
    WriteReadyness*     next;
};


#endif
