#include "ShardCatchupReader.h"
#include "System/Events/EventLoop.h"
#include "Application/ConfigState/ConfigState.h"
#include "ShardQuorumProcessor.h"
#include "ShardServer.h"

void ShardCatchupReader::Init(ShardQuorumProcessor* quorumProcessor_)
{
    quorumProcessor = quorumProcessor_;
    
    onTimeout.SetCallable(MFUNC(ShardCatchupReader, OnTimeout));
    onTimeout.SetDelay(SHARD_CATCHUP_READER_DELAY);
    
    environment = quorumProcessor->GetShardServer()->GetDatabaseManager()->GetEnvironment();

    Reset();
}

void ShardCatchupReader::Reset()
{
    isActive = false;
    shardID = 0;
    bytesReceived = 0;
    prevBytesReceived = 0;
    nextCommit = 0;
    EventLoop::Remove(&onTimeout);
}

bool ShardCatchupReader::IsActive()
{
    return isActive;
}

void ShardCatchupReader::Begin()
{
    isActive = true;
    bytesReceived = 0;
    prevBytesReceived = 0;
    nextCommit = CATCHUP_COMMIT_GRANULARITY;
    EventLoop::Add(&onTimeout);

    Log_Message("Disabling database merge for the duration of catchup (receiving)");
    environment->SetMergeEnabled(false);
}

void ShardCatchupReader::Abort()
{
    Log_Message("Catchup aborted");
    Reset();

    Log_Message("Enabling database merge");
    environment->SetMergeEnabled(true);
}

void ShardCatchupReader::OnBeginShard(CatchupMessage& msg)
{
    ConfigShard*        shard;
    ReadBuffer          firstKey;

    ASSERT(isActive);

    shard = quorumProcessor->GetShardServer()->GetConfigState()->GetShard(msg.shardID);
    ASSERT(shard != NULL);

    environment->Commit(quorumProcessor->GetQuorumID());
    environment->DeleteShard(QUORUM_DATABASE_DATA_CONTEXT, shard->shardID);
    environment->Commit(quorumProcessor->GetQuorumID());
    environment->CreateShard(quorumProcessor->GetQuorumID(),
     QUORUM_DATABASE_DATA_CONTEXT, shard->shardID,
     shard->tableID, shard->firstKey, shard->lastKey, true, STORAGE_SHARD_TYPE_STANDARD);
     
    shardID = shard->shardID;
}

void ShardCatchupReader::OnSet(CatchupMessage& msg)
{
//    Log_Debug("SET %U %B", msg.shardID, &msg.key);
    environment->Set(QUORUM_DATABASE_DATA_CONTEXT, msg.shardID, msg.key, msg.value);
    bytesReceived += msg.key.GetLength() + msg.value.GetLength();
    TryCommit();
}

void ShardCatchupReader::OnDelete(CatchupMessage& msg)
{
    environment->Delete(QUORUM_DATABASE_DATA_CONTEXT, msg.shardID, msg.key);
    bytesReceived += msg.key.GetLength();
    TryCommit();
}

void ShardCatchupReader::OnCommit(CatchupMessage& message)
{
    ASSERT(isActive);
    
    quorumProcessor->SetPaxosID(message.paxosID);

    Log_Message("Catchup complete, at paxosID = %U", message.paxosID);
    
    Reset();

    Log_Message("Enabling database merge");
    environment->SetMergeEnabled(true);
}

void ShardCatchupReader::OnAbort(CatchupMessage& /*message*/)
{
    Abort();
}

void ShardCatchupReader::TryCommit()
{
    if (bytesReceived >= nextCommit)
    {
        environment->Commit(quorumProcessor->GetQuorumID());
        nextCommit = bytesReceived + CATCHUP_COMMIT_GRANULARITY;
    }
}

void ShardCatchupReader::OnTimeout()
{
    if (bytesReceived == prevBytesReceived)
    {
        Abort();
        quorumProcessor->ContinueReplication();
    }
    else
    {
        prevBytesReceived = bytesReceived;
        EventLoop::Add(&onTimeout);
    }
}
