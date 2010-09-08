#include "ClientRequest.h"
#include "System/Buffers/Buffer.h"

bool ClientRequest::CreateDatabase(
 uint64_t commandID_, ReadBuffer& name_)
{
	type = CLIENTREQUEST_CREATE_DATABASE;
	commandID = commandID_;
	name = name_;
	return true;
}

bool ClientRequest::RenameDatabase(
 uint64_t commandID_, uint64_t databaseID_, ReadBuffer& name_)
{
	type = CLIENTREQUEST_RENAME_DATABASE;
	commandID = commandID_;
	databaseID = databaseID_;
	name = name_;
	return true;
}

bool ClientRequest::DeleteDatabase(
 uint64_t commandID_, uint64_t databaseID_)
{
	type = CLIENTREQUEST_DELETE_DATABASE;
	commandID = commandID_;
	databaseID = databaseID_;
	return true;
}

bool ClientRequest::CreateTable(
 uint64_t commandID_, uint64_t databaseID_, ReadBuffer& name_)
{
	type = CLIENTREQUEST_CREATE_TABLE;
	commandID = commandID_;
	databaseID = databaseID_;
	name = name_;
	return true;
}

bool ClientRequest::RenameTable(
 uint64_t commandID_, uint64_t databaseID_, uint64_t tableID_, ReadBuffer& name_)
{
	type = CLIENTREQUEST_RENAME_TABLE;
	commandID = commandID_;
	databaseID = databaseID_;
	tableID = tableID_;
	name = name_;
	return true;
}

bool ClientRequest::DeleteTable(
 uint64_t commandID_, uint64_t databaseID_, uint64_t tableID_)
{
	type = CLIENTREQUEST_DELETE_TABLE;
	commandID = commandID_;
	databaseID = databaseID_;
	tableID = tableID_;
	return true;
}

bool ClientRequest::Set(
 uint64_t commandID_, uint64_t databaseID_, uint64_t tableID_, ReadBuffer& key_, ReadBuffer& value_)
{
	type = CLIENTREQUEST_SET;
	commandID = commandID_;
	databaseID = databaseID_;
	tableID = tableID_;
	key = key_;
	value = value_;
	return true;
}

bool ClientRequest::Get(
 uint64_t commandID_, uint64_t databaseID_, uint64_t tableID_, ReadBuffer& key_)
{
	type = CLIENTREQUEST_GET;
	commandID = commandID_;
	databaseID = databaseID_;
	tableID = tableID_;
	key = key_;
	return true;
}

bool ClientRequest::Delete(
 uint64_t commandID_, uint64_t databaseID_, uint64_t tableID_, ReadBuffer& key_)
{
	type = CLIENTREQUEST_DELETE;
	commandID = commandID_;
	databaseID = databaseID_;
	tableID = tableID_;
	key = key_;
	return true;
}

bool ClientRequest::Read(ReadBuffer buffer)
{
	int			read;
		
	if (buffer.GetLength() < 1)
		return false;
	
	switch (buffer.GetCharAt(0))
	{
		/* Database management */
		case CLIENTREQUEST_CREATE_DATABASE:
			read = buffer.Readf("%c:%U:%#R",
			 &type, &commandID, &name);
			break;
		case CLIENTREQUEST_RENAME_DATABASE:
			read = buffer.Readf("%c:%U:%U:%#R",
			 &type, &commandID, &databaseID, &name);
			break;
		case CLIENTREQUEST_DELETE_DATABASE:
			read = buffer.Readf("%c:%U:%U",
			 &type, &commandID, &databaseID);
			break;
			
		/* Table management */
		case CLIENTREQUEST_CREATE_TABLE:
			read = buffer.Readf("%c:%U:%U:%#R",
			 &type, &commandID, &databaseID, &name);
			break;
		case CLIENTREQUEST_RENAME_TABLE:
			read = buffer.Readf("%c:%U:%U:%U:%#R",
			 &type, &commandID, &databaseID, &tableID, &name);
			break;
		case CLIENTREQUEST_DELETE_TABLE:
			read = buffer.Readf("%c:%U:%U:%U",
			 &type, &commandID, &databaseID, &tableID);
			break;
			
		/* Data operations */
		case CLIENTREQUEST_GET:
			read = buffer.Readf("%c:%U:%U:%U:%#R",
			 &type, &commandID, &databaseID, &tableID, &key);
			break;
		case CLIENTREQUEST_SET:
			read = buffer.Readf("%c:%U:%U:%U:%#R:%#R",
			 &type, &commandID, &databaseID, &tableID, &key, &value);
			break;
		case CLIENTREQUEST_DELETE:
			read = buffer.Readf("%c:%U:%U:%U:%#R",
			 &type, &commandID, &databaseID, &tableID, &key);
			break;
		default:
			return false;
	}
	
	return (read == (signed)buffer.GetLength() ? true : false);
}

bool ClientRequest::Write(Buffer& buffer)
{
	switch (type)
	{
		/* Database management */
		case CLIENTREQUEST_CREATE_DATABASE:
			buffer.Writef("%c:%U:%#R",
			 type, commandID, &name);
			return true;
		case CLIENTREQUEST_RENAME_DATABASE:
			buffer.Writef("%c:%U:%U:%#R",
			 type, commandID, databaseID, &name);
			return true;
		case CLIENTREQUEST_DELETE_DATABASE:
			buffer.Writef("%c:%U:%U",
			 type, commandID, databaseID);
			return true;

		/* Table management */
		case CLIENTREQUEST_CREATE_TABLE:
			buffer.Writef("%c:%U:%U:%#R",
			 type, commandID, databaseID, &name);
			return true;
		case CLIENTREQUEST_RENAME_TABLE:
			buffer.Writef("%c:%U:%U:%U:%#R",
			 type, commandID, databaseID, tableID, &name);
			return true;
		case CLIENTREQUEST_DELETE_TABLE:
			buffer.Writef("%c:%U:%U:%U",
			 type, commandID, databaseID, tableID);
			return true;

		/* Data operations */
		case CLIENTREQUEST_GET:
			buffer.Writef("%c:%U:%U:%U:%#R",
			 type, commandID, databaseID, tableID, &key);
			return true;
		case CLIENTREQUEST_SET:
			buffer.Writef("%c:%U:%U:%U:%#R:%#R",
			 type, commandID, databaseID, tableID, &key, &value);
			return true;
		case CLIENTREQUEST_DELETE:
			buffer.Writef("%c:%U:%U:%U:%#R",
			 type, commandID, databaseID, tableID, &key);
			return true;
		
		default:
			return false;
	}
	
	return true;
}
