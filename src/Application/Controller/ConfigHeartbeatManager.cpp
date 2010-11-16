#include "ConfigHeartbeatManager.h"
#include "System/Events/EventLoop.h"
#include "Application/Common/ContextTransport.h"
#include "ConfigActivationManager.h"
#include "Controller.h"

void ConfigHeartbeatManager::Init(Controller* controller_)
{
    controller = controller_;
    
    heartbeatTimeout.SetCallable(MFUNC(ConfigHeartbeatManager, OnHeartbeatTimeout));
    heartbeatTimeout.SetDelay(1000);
    EventLoop::Add(&heartbeatTimeout);
}

void ConfigHeartbeatManager::Shutdown()
{
    heartbeats.DeleteList();
}

void ConfigHeartbeatManager::OnHeartbeatMessage(ClusterMessage& message)
{
    QuorumPaxosID*      it;
    ConfigQuorum*       quorum;
    ConfigShardServer*  shardServer;
    
    shardServer = controller->GetDatabaseManager()->GetConfigState()->GetShardServer(message.nodeID);
    if (!shardServer)
        return;
    
    shardServer->quorumPaxosIDs = message.quorumPaxosIDs;
    
    RegisterHeartbeat(message.nodeID);
    
    FOREACH(it, message.quorumPaxosIDs)
    {
        quorum = controller->GetDatabaseManager()->GetConfigState()->GetQuorum(it->quorumID);
        if (!quorum)
            continue;
            
        if (quorum->paxosID < it->paxosID)
            quorum->paxosID = it->paxosID;
    }
}

void ConfigHeartbeatManager::OnHeartbeatTimeout()
{
    uint64_t            now;
    Heartbeat*          itHeartbeat;
    ConfigShardServer*  itShardServer;
    
    // OnHeartbeatTimeout() arrives every 1000 msec
    
    now = Now();

    // first remove nodes from the heartbeats list which have
    // not sent a heartbeat in HEARTBEAT_EXPIRE_TIME
    for (itHeartbeat = heartbeats.First(); itHeartbeat != NULL; /* incremented in body */)
    {
        if (itHeartbeat->expireTime <= now)
        {
            CONTEXT_TRANSPORT->DropConnection(itHeartbeat->nodeID);
            Log_Trace("Removing node %" PRIu64 " from heartbeats", itHeartbeat->nodeID);
            itHeartbeat = heartbeats.Delete(itHeartbeat);
        }
        else
            break;
    }
    
    if (controller->GetQuorumProcessor()->IsMaster())
    {
        FOREACH(itShardServer, controller->GetDatabaseManager()->GetConfigState()->shardServers)
        {
            if (!HasHeartbeat(itShardServer->nodeID))
                controller->GetActivationManager()->TryDeactivateShardServer(itShardServer->nodeID);
            else
                controller->GetActivationManager()->TryActivateShardServer(itShardServer->nodeID);
        }
    }
    
    EventLoop::Add(&heartbeatTimeout);    
}

bool ConfigHeartbeatManager::HasHeartbeat(uint64_t nodeID)
{
    Heartbeat* it;

    FOREACH(it, heartbeats)
    {
        if (it->nodeID == nodeID)
            return true;
    }

    return false;
}

void ConfigHeartbeatManager::RegisterHeartbeat(uint64_t nodeID)
{
    Heartbeat       *it;
    uint64_t        now;
    
    Log_Trace("Got heartbeat from %" PRIu64 "", nodeID);
    
    now = Now();
    
    FOREACH(it, heartbeats)
    {
        if (it->nodeID == nodeID)
        {
            heartbeats.Remove(it);
            it->expireTime = now + HEARTBEAT_EXPIRE_TIME;
            heartbeats.Add(it);
            return;
        }
    }
    
    it = new Heartbeat();
    it->nodeID = nodeID;
    it->expireTime = now + HEARTBEAT_EXPIRE_TIME;
    heartbeats.Add(it);
}
