#include "DataMessage.h"

void DataMessage::Set(uint64_t tableID_, ReadBuffer& key_, ReadBuffer& value_)
{
    type = DATAMESSAGE_SET;
    tableID = tableID_;
    key = key_;
    value = value_;
}

void DataMessage::Delete(uint64_t tableID_, ReadBuffer& key_)
{
    type = DATAMESSAGE_DELETE;
    tableID = tableID_;
    key = key_;
}

bool DataMessage::Read(ReadBuffer& buffer)
{
    int read;
    
    if (buffer.GetLength() < 1)
        return false;
    
    switch (buffer.GetCharAt(0))
    {
        // Data management
        case DATAMESSAGE_SET:
            read = buffer.Readf("%c:%#R:%#R",
             &type, &key, &value);
            break;
        case DATAMESSAGE_DELETE:
            read = buffer.Readf("%c:%#R",
             &type, &key);
            break;
        default:
            return false;
    }
    
    return (read == (signed)buffer.GetLength() ? true : false);
}

bool DataMessage::Write(Buffer& buffer)
{
    switch (type)
    {
        // Cluster management
        case DATAMESSAGE_SET:
            buffer.Writef("%c:%#R:%#R",
             type, &key, &value);
            break;
        case DATAMESSAGE_DELETE:
            buffer.Writef("%c:%#R",
             type, &key);
            break;
        default:
            return false;
    }
    
    return true;
}