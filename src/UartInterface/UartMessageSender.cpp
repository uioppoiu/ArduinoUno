#include "Arduino.h"
#include "UartMessageSender.h"

namespace UartMessageInterface
{
    UartMessageSender::UartMessageSender(eMessageType messageType, eCommandType commandType)
        : _seqId(0), _jsonDoc(256)
    {
        switch (messageType)
        {
        case Request:
            _jsonDoc["MessageType"] = "Request";
            break;
        case Response:
            _jsonDoc["MessageType"] = "Response";
            break;
        case Notification:
            _jsonDoc["MessageType"] = "Notification";
            break;
        case Acknowledge:
            _jsonDoc["MessageType"] = "Acknowledge";
            break;
        default:
            // throw invalid_argument("Invalid MessageType");
            return;
        }

        switch (commandType)
        {
        case Get:
            _jsonDoc["CommandType"] = "Get";
            break;
        case Set:
            _jsonDoc["CommandType"] = "Set";
            break;
        case Subscribe:
            _jsonDoc["CommandType"] = "Subscribe";
            break;
        case Unsubscribe:
            _jsonDoc["CommandType"] = "Unsubscribe";
            break;
        default:
            // throw invalid_argument("Invalid CommandType");
            return;
        }
    }

    UartMessageSender::~UartMessageSender()
    {
        _jsonDoc.garbageCollect();
    }

    void UartMessageSender::appendRequestAll(eDataType type)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.createNestedObject();

        if (type == SensorAll)
            data["Type"] = "SensorAll";
        else if (type == ControlAll)
            data["Type"] = "ControlAll";
    }

    void UartMessageSender::appendRequest(eDataType dataType, const String &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "SensorTemperature";
            break;
        case SensorCO2:
            data["Type"] = "SensorCO2";
            break;
        case SensorHumidity:
            data["Type"] = "SensorHumidity";
            break;
        case SensorConductivity:
            data["Type"] = "SensorConductivity";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendRequestAll(dataType);
            return;
        default:
            return;
        }
        data["Name"] = name;
    }

    void UartMessageSender::appendSubscribe(eDataType dataType, const String &name, unsigned int period)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "SensorTemperature";
            break;
        case SensorCO2:
            data["Type"] = "SensorCO2";
            break;
        case SensorHumidity:
            data["Type"] = "SensorHumidity";
            break;
        case SensorConductivity:
            data["Type"] = "SensorConductivity";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendSubscribeAll(dataType, period);
            return;
        default:
            return;
        }
        data["Name"] = name;
        data["Period"] = period;
    }

    void UartMessageSender::appendSubscribeAll(eDataType dataType, unsigned int period)
    {
        if (dataType != SensorAll || dataType != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.createNestedObject();

        if (dataType == SensorAll)
            data["Type"] = "SensorAll";
        else if (dataType == ControlAll)
            data["Type"] = "ControlAll";

        data["Period"] = period;        
    }

    void UartMessageSender::appendUnsubscribe(eDataType dataType, const String &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.createNestedObject();
        switch (dataType)
        {
        case SensorTemperature:
            data["Type"] = "SensorTemperature";
            break;
        case SensorCO2:
            data["Type"] = "SensorCO2";
            break;
        case SensorHumidity:
            data["Type"] = "SensorHumidity";
            break;
        case SensorConductivity:
            data["Type"] = "SensorConductivity";
            break;
        case Control1:
            data["Type"] = "Control1";
            break;
        case Control2:
            data["Type"] = "Control2";
            break;
        case DateTime:
            data["Type"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendUnsubscribeAll(dataType);
            return;
        default:
            return;
        }
        data["Name"] = name;
    }

    void UartMessageSender::appendUnsubscribeAll(eDataType dataType)
    {
        if (dataType != SensorAll || dataType != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.createNestedObject();

        if (dataType == SensorAll)
            data["Type"] = "SensorAll";
        else if (dataType == ControlAll)
            data["Type"] = "ControlAll";
    }

    void UartMessageSender::sendMessage()
    {
        String buf;
        serializeJson(_jsonDoc, buf);
        appendCheckSum(buf);

        Serial.println(buf.c_str());

        _jsonDoc.garbageCollect();
    }

}; // namespace UartMessageInterface