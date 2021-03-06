#include "ConfigQuorumContext.h"
#include "Framework/Replication/ReplicationConfig.h"
#include "Framework/Replication/PaxosLease/PaxosLeaseMessage.h"
#include "Framework/Replication/Paxos/PaxosMessage.h"
#include "Application/Common/CatchupMessage.h"
#include "ConfigQuorumProcessor.h"

void ConfigQuorumContext::Init(ConfigQuorumProcessor* quorumProcessor_, unsigned numConfigServers,
 StorageShardProxy* quorumPaxosShard, StorageShardProxy* quorumLogShard)
{
    uint64_t nodeID;
    
    quorumProcessor = quorumProcessor_;
    
    quorumID = 1;
    for (nodeID = 0; nodeID < numConfigServers; nodeID++)
        quorum.AddNode(nodeID);

    transport.SetQuorum(&quorum);
    transport.SetQuorumID(quorumID);
    
    database.Init(this, quorumPaxosShard, quorumLogShard);
    
    replicatedLog.Init(this);
    paxosLease.Init(this);
    transport.SetQuorumID(quorumID);
    highestPaxosID = 0; 

    isReplicationActive = true;

    if (quorum.GetNumNodes() > 1)
    {
        paxosLease.AcquireLease();
    }
    else
    {
        replicatedLog.OnLearnLease();
        quorumProcessor->OnLearnLease();
        quorumProcessor->OnIsLeader();
    }
}

void ConfigQuorumContext::Append(ConfigMessage* message)
{
    message->Write(nextValue);
    replicatedLog.TryAppendNextValue();
}

bool ConfigQuorumContext::IsAppending()
{
    return (nextValue.GetLength() != 0);
}

bool ConfigQuorumContext::IsLeaseOwner()
{
    if (quorum.GetNumNodes() == 1)
        return true;
    else
        return paxosLease.IsLeaseOwner();
}

bool ConfigQuorumContext::IsLeaseKnown()
{
    if (quorum.GetNumNodes() == 1)
        return true;
    else
        return paxosLease.IsLeaseKnown();
}

uint64_t ConfigQuorumContext::GetLeaseOwner()
{
    if (quorum.GetNumNodes() == 1)
        return MY_NODEID;
    else
        return paxosLease.GetLeaseOwner();
}

bool ConfigQuorumContext::IsLeader()
{
    if (quorum.GetNumNodes() == 1)
        return true;
    else
        return IsLeaseOwner() && replicatedLog.IsMultiPaxosEnabled();
}

void ConfigQuorumContext::OnLearnLease()
{
    replicatedLog.OnLearnLease();
    quorumProcessor->OnLearnLease();
}

void ConfigQuorumContext::OnLeaseTimeout()
{
    nextValue.Clear();
    replicatedLog.OnLeaseTimeout();
    quorumProcessor->OnLeaseTimeout();
}

void ConfigQuorumContext::OnIsLeader()
{
    quorumProcessor->OnIsLeader();
}

uint64_t ConfigQuorumContext::GetQuorumID()
{
    return quorumID;
}

void ConfigQuorumContext::SetPaxosID(uint64_t paxosID)
{
    replicatedLog.SetPaxosID(paxosID);
}

uint64_t ConfigQuorumContext::GetPaxosID()
{
    return replicatedLog.GetPaxosID();
}

uint64_t ConfigQuorumContext::GetHighestPaxosID()
{
    return highestPaxosID;
}

uint64_t ConfigQuorumContext::GetLastLearnChosenTime()
{
    return replicatedLog.GetLastLearnChosenTime();
}

uint64_t ConfigQuorumContext::GetReplicationThroughput()
{
    return replicatedLog.GetReplicationThroughput();
}

Quorum* ConfigQuorumContext::GetQuorum()
{
    return &quorum;
}

QuorumDatabase* ConfigQuorumContext::GetDatabase()
{
    return &database;
}

QuorumTransport* ConfigQuorumContext::GetTransport()
{
    return &transport;
}

bool ConfigQuorumContext::UseSyncCommit()
{
    return true;
}

bool ConfigQuorumContext::UseProposeTimeouts()
{
    return true;
}

bool ConfigQuorumContext::UseCommitChaining()
{
    return false;
}

bool ConfigQuorumContext::AlwaysUseDatabaseCatchup()
{
    return true;
}

bool ConfigQuorumContext::IsPaxosBlocked()
{
    return false;
}

Buffer& ConfigQuorumContext::GetNextValue()
{
    return nextValue;
}

void ConfigQuorumContext::OnStartProposing()
{
}

void ConfigQuorumContext::OnAppend(uint64_t paxosID, Buffer& value, bool ownAppend)
{
    bool            ret;
    ReadBuffer      rbValue;
    ConfigMessage   message;

    nextValue.Clear();

    rbValue.Wrap(value);
    ret = message.Read(rbValue);
    ASSERT(ret);
    quorumProcessor->OnAppend(paxosID, message, ownAppend);
}

void ConfigQuorumContext::OnMessage(ReadBuffer buffer)
{
    char proto;
    
    Log_Trace("%R", &buffer);

    if (buffer.GetLength() < 2)
        ASSERT_FAIL();

    proto = buffer.GetCharAt(0);
    ASSERT(buffer.GetCharAt(1) == ':');
    
    switch(proto)
    {
        case PAXOSLEASE_PROTOCOL_ID:        // 'L':
            OnPaxosLeaseMessage(buffer);
            break;
        case PAXOS_PROTOCOL_ID:             // 'P':
            OnPaxosMessage(buffer);
            break;
        case CATCHUP_PROTOCOL_ID:           // 'C'
            OnCatchupMessage(buffer);
            break;
        default:
            ASSERT_FAIL();
            break;
    }
}

void ConfigQuorumContext::OnStartCatchup()
{
    quorumProcessor->OnStartCatchup();
}

void ConfigQuorumContext::OnCatchupStarted()
{
    replicatedLog.OnCatchupStarted();
}

void ConfigQuorumContext::OnCatchupComplete(uint64_t paxosID)
{
    replicatedLog.OnCatchupComplete(paxosID);
}

void ConfigQuorumContext::OnAppendComplete()
{
    replicatedLog.OnAppendComplete();
}

void ConfigQuorumContext::StopReplication()
{
    // TODO: xxx
}

void ConfigQuorumContext::ContinueReplication()
{
    // TODO: xxx
}

bool ConfigQuorumContext::IsWaitingOnAppend()
{
    return replicatedLog.IsWaitingOnAppend();
}

void ConfigQuorumContext::OnPaxosLeaseMessage(ReadBuffer buffer)
{
    PaxosLeaseMessage msg;

    Log_Trace("%R", &buffer);
    
    msg.Read(buffer);
    if (msg.type == PAXOSLEASE_PREPARE_REQUEST || msg.type == PAXOSLEASE_LEARN_CHOSEN)
    {
        if (isReplicationActive)
        {
            RegisterPaxosID(msg.paxosID);
            replicatedLog.RegisterPaxosID(msg.paxosID, msg.nodeID);
        }
    }
    paxosLease.OnMessage(msg);
    // TODO: right now PaxosLeaseLearner will not call back
}


void ConfigQuorumContext::OnPaxosMessage(ReadBuffer buffer)
{
    PaxosMessage msg;
 
    if (!isReplicationActive)
        return;

    msg.Read(buffer);

    if (msg.IsPaxosRequest() || msg.IsPaxosResponse() || msg.IsLearn())
    {
        if (!quorum.IsMember(msg.nodeID))
        {
            Log_Message("Dropping paxos msg from %U because that node is not a quourm member", msg.nodeID);
            OnMessageProcessed();
            return;
        }
    }

    RegisterPaxosID(msg.paxosID);
    replicatedLog.RegisterPaxosID(msg.paxosID, msg.nodeID);
    replicatedLog.OnMessage(msg);
}

void ConfigQuorumContext::OnCatchupMessage(ReadBuffer buffer)
{
    CatchupMessage msg;
    
    msg.Read(buffer);
    quorumProcessor->OnCatchupMessage(msg);
}

void ConfigQuorumContext::RegisterPaxosID(uint64_t paxosID)
{
    if (paxosID > highestPaxosID)
        highestPaxosID = paxosID;
}
