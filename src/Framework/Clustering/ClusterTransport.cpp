#include "ClusterTransport.h"

ClusterTransport::~ClusterTransport()
{
    conns.DeleteList();
    writeReadynessList.DeleteList();
}

void ClusterTransport::Init(Endpoint& endpoint_)
{
    endpoint = endpoint_;
    if (!server.Init(endpoint.GetPort()))
        STOP_FAIL(1, "Cannot bind on cluster port: %s", endpoint.ToString());
    server.SetTransport(this);
    awaitingNodeID = true;
    nodeID = UNDEFINED_NODEID;
    clusterID = 0;
}

void ClusterTransport::SetSelfNodeID(uint64_t nodeID_)
{
    nodeID = nodeID_;
    awaitingNodeID = false;
    ReconnectAll(); // so we establish "regular" connections with the nodeID
    server.Listen();
}

void ClusterTransport::SetClusterID(uint64_t clusterID_)
{
    clusterID = clusterID_;
}

bool ClusterTransport::IsAwaitingNodeID()
{
    return awaitingNodeID;
}

uint64_t ClusterTransport::GetSelfNodeID()
{
    return nodeID;
}

Endpoint& ClusterTransport::GetSelfEndpoint()
{
    return endpoint;
}

uint64_t ClusterTransport::GetClusterID()
{
    return clusterID;
}

unsigned ClusterTransport::GetNumConns()
{
    return conns.GetLength();
}

unsigned ClusterTransport::GetNumWriteReadyness()
{
    return writeReadynessList.GetLength();
}

void ClusterTransport::AddConnection(uint64_t nodeID, Endpoint& endpoint)
{
    ClusterConnection* conn;
    
    conn = GetConnection(nodeID);
    if (conn != NULL)
        return;

    conn = new ClusterConnection;
    conn->SetTransport(this);
    conn->SetNodeID(nodeID);
    conn->SetEndpoint(endpoint);
    conn->Connect();
    conns.Append(conn);

    if (!awaitingNodeID && nodeID == this->nodeID)
        Log_Trace("connecting to self");
}

bool ClusterTransport::HasConnection(uint64_t nodeID)
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        if (it->GetNodeID() == nodeID)
            return true;
    }
    
    return false;
}

bool ClusterTransport::IsConnected(uint64_t nodeID)
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        if (it->GetNodeID() == nodeID)
        {
            if (it->GetProgress() == ClusterConnection::READY)
                return true;
            else
                return false;
        }
    }
    
    return false;
}

Endpoint& ClusterTransport::GetEndpoint(uint64_t nodeID)
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        if (it->GetNodeID() == nodeID)
            return it->endpoint;
    }
    
    ASSERT_FAIL();
    // never gets here, this is only here for suppressing VC++ warning
    STOP_FAIL(1, "Program error in ClusterTransport::GetEndpoint");
}

bool ClusterTransport::SetConnectionNodeID(Endpoint& endpoint, uint64_t nodeID)
{
    ClusterConnection* it;
    ClusterConnection* tdb;
    
    for (it = conns.First(); it != NULL;)
    {
        if (it->GetEndpoint() == endpoint)
        {
            if (it->GetProgress() != ClusterConnection::AWAITING_NODEID)
            {
                // the node had its db cleared, and this is our old connection to it
                tdb = it;
                it = conns.Next(it);
                DeleteConnection(tdb);
            }
            else
            {
                it->SetNodeID(nodeID);
                it->SetProgress(ClusterConnection::READY);
                return true;
            }
        }
        else
            it = conns.Next(it);
    }
    
    return false;
}

void ClusterTransport::SendMessage(uint64_t nodeID, Buffer& prefix, Message& msg)
{
    bool                ret;
    ClusterConnection*  conn;
    
    conn = GetConnection(nodeID);
    
    if (!conn)
    {
        Log_Trace("no connection to nodeID %U", nodeID);
        return;
    }
    
    if (conn->GetProgress() != ClusterConnection::READY)
    {
        Log_Trace("connection to %U has progress: %d", nodeID, conn->GetProgress());
        return;
    }
    
    msgBuffer.Clear();
    ret = msg.Write(msgBuffer);
    ASSERT(ret);
    ASSERT(msgBuffer.GetLength() > 0);
    conn->Write(prefix, msgBuffer);
}

void ClusterTransport::DropConnection(uint64_t nodeID)
{
    ClusterConnection* conn;
    
    conn = GetConnection(nodeID);
    
    if (!conn)
        return;
        
    DeleteConnection(conn);
}

void ClusterTransport::DropConnection(Endpoint endpoint)
{
    ClusterConnection* conn;
    
    conn = GetConnection(endpoint);
    
    if (!conn)
        return;
        
    DeleteConnection(conn);
}

bool ClusterTransport::GetNextWaiting(Endpoint& endpoint)
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        if (it->GetProgress() == ClusterConnection::AWAITING_NODEID)
        {
            endpoint = it->GetEndpoint();
            return true;
        }
    }
    
    return false;
}

void ClusterTransport::RegisterWriteReadyness(WriteReadyness* wr)
{
    ClusterConnection*  itConnection;

    // if not in list, append
    if (wr->next == wr)
        writeReadynessList.Append(wr);
    
    FOREACH (itConnection, conns)
    {
        if (itConnection->GetNodeID() == wr->nodeID &&
         itConnection->GetProgress() == ClusterConnection::READY)
        {
            Call(wr->callable);
            return;
        }
    }
}

void ClusterTransport::UnregisterWriteReadyness(WriteReadyness* wr)
{
    // if in list, remove
    if (wr->next != wr)
        writeReadynessList.Remove(wr);
}

void ClusterTransport::AddConnection(ClusterConnection* conn)
{
    conns.Append(conn);
}

ClusterConnection* ClusterTransport::GetConnection(uint64_t nodeID)
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        if (it->GetNodeID() == nodeID && it->GetProgress() != ClusterConnection::AWAITING_NODEID)
            return it;
    }
    
    return NULL;
}

ClusterConnection* ClusterTransport::GetConnection(Endpoint& endpoint)
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        if (it->GetEndpoint() == endpoint)
            return it;
    }
    
    return NULL;
}

void ClusterTransport::DeleteConnection(ClusterConnection* conn)
{
    conn->Close();

    if (conn->next != conn)
        conns.Remove(conn);

    delete conn;
}

void ClusterTransport::ReconnectAll()
{
    ClusterConnection* it;
    
    FOREACH (it, conns)
    {
        it->Close();
        it->Connect();
    }
}

void ClusterTransport::OnWriteReadyness(ClusterConnection* conn)
{
    WriteReadyness* it;
    
    ASSERT(conn->progress == ClusterConnection::READY);
    
    FOREACH (it, writeReadynessList)
    {
        if (it->nodeID == conn->nodeID)
        {
            Call(it->callable);
            return;
        }
    }
}
