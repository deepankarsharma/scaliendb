#ifndef STORAGEMEMOKEYVALUE_H
#define STORAGEMEMOKEYVALUE_H

#include "System/Buffers/Buffer.h"
#include "System/Containers/InTreeMap.h"
#include "StorageKeyValue.h"

/*
===============================================================================================

 StorageMemoKeyValue

===============================================================================================
*/

class StorageMemoKeyValue : public StorageKeyValue
{
public:
    typedef InTreeNode<StorageMemoKeyValue> TreeNode;

    StorageMemoKeyValue();
    ~StorageMemoKeyValue();

    void            Set(ReadBuffer key, ReadBuffer value);
    void            Delete(ReadBuffer key);
    
    char            GetType();
    ReadBuffer&     GetKey();
    ReadBuffer&     GetValue();
    uint32_t        GetLength();

    TreeNode        treeNode;

private:
    ReadBuffer      key;
    ReadBuffer      value;
    Buffer*         keyBuffer;
    Buffer*         valueBuffer;
};

#endif
