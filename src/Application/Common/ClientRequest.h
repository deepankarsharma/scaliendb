#ifndef CLIENTREQUEST_H
#define CLIENTREQUEST_H

#include "System/Containers/List.h"
#include "System/Buffers/ReadBuffer.h"
#include "ClientResponse.h"

#define CLIENTREQUEST_UNDEFINED                         ' '
#define CLIENTREQUEST_TRANSACTIONAL                     'T'
#define CLIENTREQUEST_GET_MASTER                        'm'
#define CLIENTREQUEST_GET_MASTER_HTTP                   'H'
#define CLIENTREQUEST_GET_CONFIG_STATE                  'A'
#define CLIENTREQUEST_UNREGISTER_SHARDSERVER            'w'
#define CLIENTREQUEST_CREATE_QUORUM                     'Q'
#define CLIENTREQUEST_RENAME_QUORUM                     'q'
#define CLIENTREQUEST_DELETE_QUORUM                     'W'
#define CLIENTREQUEST_ADD_SHARDSERVER_TO_QUORUM         'n'
#define CLIENTREQUEST_REMOVE_SHARDSERVER_FROM_QUORUM    'b'
#define CLIENTREQUEST_ACTIVATE_SHARDSERVER              'N'
#define CLIENTREQUEST_SET_PRIORITY                      'P'
#define CLIENTREQUEST_CREATE_DATABASE                   'C'
#define CLIENTREQUEST_RENAME_DATABASE                   'R'
#define CLIENTREQUEST_DELETE_DATABASE                   'D'
#define CLIENTREQUEST_CREATE_TABLE                      'c'
#define CLIENTREQUEST_RENAME_TABLE                      'r'
#define CLIENTREQUEST_DELETE_TABLE                      'd'
#define CLIENTREQUEST_TRUNCATE_TABLE                    't'
#define CLIENTREQUEST_FREEZE_TABLE                      'F'
#define CLIENTREQUEST_UNFREEZE_TABLE                    'f'
#define CLIENTREQUEST_FREEZE_DATABASE                   '7'
#define CLIENTREQUEST_UNFREEZE_DATABASE                 '8'
#define CLIENTREQUEST_GET                               'G'
#define CLIENTREQUEST_SET                               'S'
#define CLIENTREQUEST_SET_IF_NOT_EXISTS                 'I'
#define CLIENTREQUEST_TEST_AND_SET                      's'
#define CLIENTREQUEST_TEST_AND_DELETE                   'i'
#define CLIENTREQUEST_GET_AND_SET                       'g'
#define CLIENTREQUEST_ADD                               'a'
#define CLIENTREQUEST_APPEND                            'p'
#define CLIENTREQUEST_DELETE                            'X'
#define CLIENTREQUEST_REMOVE                            'x'
#define CLIENTREQUEST_SEQUENCE_SET                      'y'
#define CLIENTREQUEST_SEQUENCE_NEXT                     'Y'
#define CLIENTREQUEST_LIST_KEYS                         'L'
#define CLIENTREQUEST_LIST_KEYVALUES                    'l'
#define CLIENTREQUEST_COUNT                             'O'
#define CLIENTREQUEST_SPLIT_SHARD                       'h'
#define CLIENTREQUEST_MIGRATE_SHARD                     'M'
#define CLIENTREQUEST_START_TRANSACTION                 '<'
#define CLIENTREQUEST_COMMIT_TRANSACTION                '>'
#define CLIENTREQUEST_ROLLBACK_TRANSACTION              '~'

class ClientSession; // forward

/*
===============================================================================================

 ClientRequest

===============================================================================================
*/

class ClientRequest
{
public:
    ClientRequest();
    
    void            Init();
    void            Clear();
    void            OnComplete(bool last = true);

    bool            IsControllerRequest();
    bool            IsShardServerRequest();
    bool            IsReadRequest();
    bool            IsList();
    bool            IsTransaction();
    bool            IsActive();
    
    // Master query
    void            GetMaster(
                     uint64_t commandID);
    void            GetMasterHTTP(
                     uint64_t commandID);

    // Get config state: databases, tables, shards, quora
    void            GetConfigState(
                     uint64_t commandID, uint64_t changeTimeout = 0);

    // Shard servers
    void            UnregisterShardServer(
                     uint64_t commandID, uint64_t nodeID);

    // Quorum management
    void            CreateQuorum(
                     uint64_t commandID, ReadBuffer& name, List<uint64_t>& nodes);
    void            RenameQuorum(
                     uint64_t commandID, uint64_t quorumID, ReadBuffer& name);
    void            DeleteQuorum(
                     uint64_t commandID, uint64_t quorumID);
    void            AddShardServerToQuorum(
                     uint64_t commandID, uint64_t quorumID, uint64_t nodeID);
    void            RemoveShardServerFromQuorum(
                     uint64_t commandID, uint64_t quorumID, uint64_t nodeID);
    void            ActivateShardServer(
                     uint64_t commandID, uint64_t nodeID);
    void            SetPriority(
                     uint64_t commandID, uint64_t quorumID, uint64_t nodeID, uint64_t priority);
    
    // Database management
    void            CreateDatabase(
                     uint64_t commandID, ReadBuffer& name);
    void            RenameDatabase(
                     uint64_t commandID, uint64_t databaseID, ReadBuffer& name);
    void            DeleteDatabase(
                     uint64_t commandID, uint64_t databaseID);
    
    // Table management
    void            CreateTable(
                     uint64_t commandID, uint64_t databaseID, uint64_t quorumID, ReadBuffer& name);
    void            RenameTable(
                     uint64_t commandID, uint64_t tableID, ReadBuffer& name);
    void            DeleteTable(
                     uint64_t commandID, uint64_t tableID);
    void            TruncateTable(
                     uint64_t commandID, uint64_t tableID);
    void            SplitShard(
                     uint64_t commandID, uint64_t shardID, ReadBuffer& key);
    void            FreezeTable(
                     uint64_t commandID, uint64_t tableID);
    void            UnfreezeTable(
                     uint64_t commandID, uint64_t tableID);
    void            FreezeDatabase(
                     uint64_t commandID, uint64_t databaseID);
    void            UnfreezeDatabase(
                     uint64_t commandID, uint64_t databaseID);
    void            MigrateShard(
                     uint64_t commandID, uint64_t shardID, uint64_t quorumID);
    
    // Data manipulations
    void            Get(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key);
    void            Set(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, ReadBuffer& value);
    void            SetIfNotExists(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, ReadBuffer& value);
    void            TestAndSet(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, ReadBuffer& test, ReadBuffer& value);
    void            TestAndDelete(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, ReadBuffer& test);
    void            GetAndSet(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, ReadBuffer& value);
    void            Add(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, int64_t number);
    void            Append(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, ReadBuffer& value);
    void            Delete(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key);    
    void            Remove(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key);
    void            SequenceSet(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key, uint64_t sequence);
    void            SequenceNext(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, ReadBuffer& key);
    void            ListKeys(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID, 
                     ReadBuffer& startKey, ReadBuffer& endKey, ReadBuffer& prefix,
                     unsigned count, bool forwardDirection);
    void            ListKeyValues(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID,
                     ReadBuffer& startKey, ReadBuffer& endKey, ReadBuffer& prefix,
                     unsigned count, bool forwardDirection);
    void            Count(
                     uint64_t commandID, uint64_t configPaxosID,
                     uint64_t tableID,
                     ReadBuffer& startKey, ReadBuffer& endKey, ReadBuffer& prefix,
                     bool forwardDirection);

    // Transactions
    void            StartTransaction(uint64_t commandID, uint64_t configPaxosID,
                     uint64_t quorumID, ReadBuffer& majorKey);
    void            CommitTransaction(uint64_t commandID, uint64_t quorumID);
    void            RollbackTransaction(uint64_t commandID, uint64_t quorumID);

    // Variables
    ClientResponse  response;
    ClientSession*  session;

    ClientRequest*  prev;
    ClientRequest*  next;

    bool            forwardDirection;
    bool            findByLastKey;
    bool            transactional;
    char            type;
    uint64_t        commandID;
    uint64_t        quorumID;
    uint64_t        databaseID;
    uint64_t        tableID;
    uint64_t        shardID;
    uint64_t        nodeID;
    uint64_t        paxosID;
    uint64_t        configPaxosID;
    uint64_t        priority;
    int64_t         number;
    uint64_t        sequence;
    uint64_t        count;
    Buffer          name;
    Buffer          key;
    Buffer          prefix;
    Buffer          value;
    Buffer          test;
    Buffer          endKey;
    List<uint64_t>  nodes;
    uint64_t        changeTimeout;
    uint64_t        lastChangeTime;
    
};

#endif
