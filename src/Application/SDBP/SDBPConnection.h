#ifndef SDBPCONNECTION_H
#define SDBPCONNECTION_H

#include "Framework/Messaging/MessageConnection.h"
#include "Application/Common/ClientSession.h"
#include "Application/Common/ClientResponse.h"

#define SDBP_MAX_QUEUED_BYTES        (100*KiB)

class SDBPContext;
class SDBPServer;
class ClientRequest;

/*
===============================================================================================

 SDBPConnection

===============================================================================================
*/

class SDBPConnection : public MessageConnection, public ClientSession
{
public:
    SDBPConnection();
    ~SDBPConnection();
    
    void                Init(SDBPServer* server);
    void                SetContext(SDBPContext* context);

    // ========================================================================================
    // MessageConnection interface:
    //
    // OnMessage() returns whether the connection was closed and deleted
    bool                OnMessage(ReadBuffer& msg);
    // Must overrise OnWrite(), to handle closeAfterSend()
    void                OnWrite();
    // Must override OnClose() to prevent the default behaviour, which is to call Close(),
    // in case numPendingOps > 0
    void                OnClose();
    // ========================================================================================

    // ========================================================================================
    // ClientSession interface
    //
    virtual void        OnComplete(ClientRequest* request, bool last);
    virtual bool        IsActive();
    // ========================================================================================

    void                UseKeepAlive(bool useKeepAlive);
    void                OnKeepAlive();

private:
    SDBPServer*         server;
    SDBPContext*        context;
    Countdown           onKeepAlive;
    Endpoint            remoteEndpoint;
    unsigned            numPending;
    unsigned            numCompleted;
    uint64_t            connectTimestamp;
};

#endif
