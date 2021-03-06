#ifndef STORAGECONFIG_H
#define STORAGECONFIG_H

#include "System/Common.h"

/*
===============================================================================================

 StorageConfig

===============================================================================================
*/

class StorageConfig
{
public:
    void        SetChunkSize(uint64_t chunkSize);
    void        SetLogSegmentSize(uint64_t logSegmentSize);
    void        SetFileChunkCacheSize(uint64_t fileChunkCacheSize);
    void        SetMemoChunkCacheSize(uint64_t memoChunkCacheSize);
    void        SetLogSize(uint64_t logSize);
    void        SetMergeBufferSize(uint64_t mergeBufferSize);
    void        SetMergeYieldFactor(uint64_t mergeYieldFactor);
    void        SetSyncGranularity(uint64_t syncGranularity);
    void        SetWriteGranularity(uint64_t writeGranularity);
    void        SetReplicatedLogSize(uint64_t replicatedLogSize);
    void		SetAbortWaitingListsNum(uint64_t abortWaitingListsNum);
    void        SetListDataPageCacheSize(uint64_t listDataPageCacheSize);
    void        SetMaxChunkPerShard(unsigned maxChunkPerShard);

    uint64_t    GetChunkSize();
    uint64_t    GetLogSegmentSize();
    uint64_t    GetFileChunkCacheSize();
    uint64_t    GetMemoChunkCacheSize();
    uint64_t    GetNumLogSegments();
    uint64_t    GetMergeBufferSize();
    uint64_t    GetMergeYieldFactor();
    uint64_t    GetSyncGranularity();
    uint64_t    GetWriteGranularity();
    uint64_t    GetReplicatedLogSize();
    uint64_t	GetAbortWaitingListsNum();
    uint64_t    GetListDataPageCacheSize();
    unsigned    GetMaxChunkPerShard();

private:
    uint64_t    chunkSize;
    uint64_t    logSegmentSize;
    uint64_t    fileChunkCacheSize;
    uint64_t    memoChunkCacheSize;
    uint64_t    numLogSegments;
    uint64_t    mergeBufferSize;
    uint64_t    mergeYieldFactor;
    uint64_t    syncGranularity;
    uint64_t    writeGranularity;
    uint64_t    replicatedLogSize;
    uint64_t	abortWaitingListsNum;
    uint64_t    listDataPageCacheSize;
    unsigned    maxChunkPerShard;
};

#endif
