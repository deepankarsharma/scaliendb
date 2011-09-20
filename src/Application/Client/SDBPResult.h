#ifndef SDBPRESULT_H
#define SDBPRESULT_H

#include "System/Containers/InTreeMap.h"
#include "Application/Common/ClientResponse.h"
#include "SDBPClientRequest.h"

namespace SDBPClient
{

/*
===============================================================================================

 SDBPClient::Result

===============================================================================================
*/

class Result
{
public:
    Result();
    ~Result();
    
    void            Close();
    
    void            Begin();
    void            Next();
    bool            IsEnd();

    bool            IsFinished();

    bool            AppendRequest(Request* req);
    bool            AppendRequestResponse(ClientResponse* resp);
    void            RemoveRequest(Request* req);

    int             GetCommandStatus();
    int             GetTransportStatus();
    void            SetTransportStatus(int status);
    void            SetConnectivityStatus(int status);
    int             GetTimeoutStatus();
    void            SetTimeoutStatus(int status);

    unsigned        GetNumNodes();
    uint64_t        GetNodeID(unsigned n);
    unsigned        GetElapsedTime();

    int             GetKey(ReadBuffer& key);
    int             GetValue(ReadBuffer& value);
    int             GetSignedNumber(int64_t& number);
    int             GetNumber(uint64_t& number);
    int             IsConditionalSuccess(bool& isConditionalSuccess);
    
    int             GetDatabaseID(uint64_t& databaseID);
    int             GetTableID(uint64_t& tableID);

    unsigned        GetRequestCount();
    Request*        GetRequestCursor();

    void            HandleRequestResponse(Request* req, ClientResponse* resp);

    typedef InTreeMap<Request> RequestMap;
    
    RequestMap          requests;
private:
    int                 transportStatus;
    int                 timeoutStatus;
    int                 connectivityStatus;
    unsigned            numCompleted;
    Request*            requestCursor;
    ClientResponse**    responseCursor;
    unsigned            responsePos;

public:
    bool                proxied;
    ReadBuffer          proxiedValue;
};

};  // namespace

#endif
