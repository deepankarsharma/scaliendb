#include "ClientResponse.h"
#include "System/Buffers/Buffer.h"

ClientResponse::ClientResponse()
{
    valueBuffer = NULL;
}

ClientResponse::~ClientResponse()
{
    delete valueBuffer;
}

void ClientResponse::CopyValue()
{
    if (valueBuffer == NULL)
        valueBuffer = new Buffer;

    valueBuffer->Write(value.GetBuffer(), value.GetLength());
    value.Wrap(*valueBuffer);
}

void ClientResponse::Transfer(ClientResponse& other)
{
    other.type = type;
    other.number = number;
    other.commandID = commandID;
    other.value = value;
    other.valueBuffer = valueBuffer;
    other.configState = configState;
    
    valueBuffer = NULL;
}

bool ClientResponse::OK()
{
    type = CLIENTRESPONSE_OK;
    return true;
}

bool ClientResponse::Number(uint64_t number_)
{
    type = CLIENTRESPONSE_NUMBER;
    number = number_;
    return true;
}

bool ClientResponse::Value(ReadBuffer& value_)
{
    type = CLIENTRESPONSE_VALUE;
    value = value_;
    return true;
}

bool ClientResponse::ConfigStateResponse(ConfigState& configState_)
{
    type = CLIENTRESPONSE_CONFIG_STATE;
    configState = configState_;
    return true;
}

bool ClientResponse::NoService()
{
    type = CLIENTRESPONSE_NOSERVICE;
    return true;
}

bool ClientResponse::Failed()
{
    type = CLIENTRESPONSE_FAILED;
    return true;
}
