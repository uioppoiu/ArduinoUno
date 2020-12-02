#include "Arduino.h"
#include "UartMessageSender.h"

namespace UartMessageInterface
{

    UartMessageSender::UartMessageSender(const unsigned char msgId)
    : _header((MsgCommonHeader*)_messageBuffer)
    {
        memset(_messageBuffer, 0x00, sizeof(_messageBuffer));
        _header->msgId = msgId;
        _header->msgSize = sizeof(MsgCommonHeader);
    }
    
    UartMessageSender::~UartMessageSender()
    {}

    void UartMessageSender::setSeqId(uint32_t seqId)
    {
        _header->seqId = seqId;
    }

    void UartMessageSender::sendMessage()
    {
        uint8_t checkSum = getCheckSum(_messageBuffer, _header->msgSize);
        _messageBuffer[_header->msgSize] = checkSum;
        _header->msgSize += 1;

        Serial.write("<BEGIN>");
        Serial.write(_messageBuffer, _header->msgSize);
        Serial.write("<END>");

        Serial.flush();
    }

    void UartMessageSender::appendRequestGetData(unsigned char dataType, const char *name, size_t sizeOfName)
    {
        RequestGetData data;
        memset(&data, 0x00, sizeof(RequestGetData));

        data.type = dataType;

        memset(data.name, 0x00, sizeof(data.name));
        size_t strSize = (sizeOfName > sizeof(data.name)) ? (sizeof(data.name)) : sizeOfName;
        memcpy(data.name, name, strSize);

        appendData(data);
    }

    void UartMessageSender::appendResponseGetData(unsigned char dataType, const char *name, size_t sizeOfName, uint32_t value)
    {
        ResponseGetData data;
        memset(&data, 0x00, sizeof(ResponseGetData));

        data.type = dataType;

        memset(data.name, 0x00, sizeof(data.name));
        size_t strSize = (sizeOfName > sizeof(data.name)) ? (sizeof(data.name)) : sizeOfName;
        memcpy(data.name, name, strSize);

        data.value = value;

        appendData(data);
    }

    void UartMessageSender::appendNotificationData(unsigned char dataType, const char *name, size_t sizeOfName, uint32_t value)
    {
        appendResponseGetData(dataType, name, sizeOfName, value);
    }

    void UartMessageSender::appendRequestSetData(unsigned char dataType, const char *name, size_t sizeOfName, uint32_t value)
    {
        appendResponseGetData(dataType, name, sizeOfName, value);
    }

    void UartMessageSender::appendSubscribeData(unsigned char dataType, const char *name, size_t sizeOfName)
    {
        appendRequestGetData(dataType, name, sizeOfName);
    }

    void UartMessageSender::appendUnsubscribeData(unsigned char dataType, const char *name, size_t sizeOfName)
    {
        appendRequestGetData(dataType, name, sizeOfName);
    }

}; // namespace UartMessageInterface