#ifndef STORAGESHARDPROXY_H
#define STORAGESHARDPROXY_H

#include "System/Platform.h"
#include "System/Buffers/ReadBuffer.h"

class StorageEnvironment;

class StorageShardProxy
{
public:
    void                    Init(StorageEnvironment* environment, 
                             uint16_t contextID, uint64_t shardID);
    
    bool                    Get(ReadBuffer key, ReadBuffer& value);
    bool                    Set(ReadBuffer key, ReadBuffer value);
    bool                    Delete(ReadBuffer key);
    
    StorageEnvironment*     GetEnvironment();
    
private:
    StorageEnvironment*     environment;
    uint16_t                contextID;
    uint64_t                shardID;
};

#endif
