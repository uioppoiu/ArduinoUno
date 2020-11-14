#include "Arduino.h"
#include "UartMessageSender.h"

namespace UartMessageInterface
{
    UartMessageSender::UartMessageSender(eMessageType messageType, eCommandType commandType)
        : _seqId(0), _jsonDoc(512)
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

    void UartMessageSender::appendRequestAll(eDataType type)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");

        if (type == SensorAll)
            dataArr.add("SensorAll");
        else if (type == ControlAll)
            dataArr.add("ControlAll");
    }

    void UartMessageSender::appendRequest(eDataType dataType, const std::string &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        switch (dataType)
        {
        case SensorTemperature:
            dataArr.add("SensorTemperature");
            break;
        case SensorCO2:
            dataArr.add("SensorCO2");
            break;
        case SensorHumidity:
            dataArr.add("SensorHumidity");
            break;
        case SensorConductivity:
            dataArr.add("SensorConductivity");
            break;
        case Control1:
            dataArr.add("Control1");
            break;
        case Control2:
            dataArr.add("Control2");
            break;
        case DateTime:
            dataArr.add("DateTime");
            break;
        case SensorAll:
        case ControlAll:
            appendRequestAll(dataType);
            return;
        default:
            return;
        }
    }

    void UartMessageSender::appendSubscribe(eDataType dataType, const std::string &name, unsigned int period)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        JsonObject data = dataArr.addElement();
        switch (dataType)
        {
        case SensorTemperature:
            data["Name"] = "SensorTemperature";
            break;
        case SensorCO2:
            data["Name"] = "SensorCO2";
            break;
        case SensorHumidity:
            data["Name"] = "SensorHumidity";
            break;
        case SensorConductivity:
            data["Name"] = "SensorConductivity";
            break;
        case Control1:
            data["Name"] = "Control1";
            break;
        case Control2:
            data["Name"] = "Control2";
            break;
        case DateTime:
            data["Name"] = "DateTime";
            break;
        case SensorAll:
        case ControlAll:
            appendRequestAll(dataType);
            return;
        default:
            return;
        }
        data["Period"] = period;
    }

    void UartMessageSender::appendSubscribeAll(eDataType dataType, unsigned int period)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");
        JsonObject data = dataArr.addElement();

        if (dataType == SensorAll)
            data["Name"] = "SensorAll";
        else if (dataType == ControlAll)
            data["Name"] = "ControlAll";

        data["Period"] = period;        
    }

    void UartMessageSender::appendUnsubscribe(eDataType dataType, const std::string &name)
    {
        if (!_jsonDoc.containsKey("Data"))
        {
            _jsonDoc.createNestedArray("Data");
        }

        JsonArray dataArr = _jsonDoc.getMember("Data");
        switch (dataType)
        {
        case SensorTemperature:
            dataArr.add("SensorTemperature");
            break;
        case SensorCO2:
            dataArr.add("SensorCO2");
            break;
        case SensorHumidity:
            dataArr.add("SensorHumidity");
            break;
        case SensorConductivity:
            dataArr.add("SensorConductivity");
            break;
        case Control1:
            dataArr.add("Control1");
            break;
        case Control2:
            dataArr.add("Control2");
            break;
        case DateTime:
            dataArr.add("DateTime");
            break;
        case SensorAll:
        case ControlAll:
            appendUnsubscribeAll(dataType);
            return;
        default:
            return;
        }
    }

    void UartMessageSender::appendUnsubscribeAll(eDataType type)
    {
        if (type != SensorAll || type != ControlAll)
            return;

        _jsonDoc.remove("Data");
        JsonArray dataArr = _jsonDoc.createNestedArray("Data");

        if (type == SensorAll)
            dataArr.add("SensorAll");
        else if (type == ControlAll)
            dataArr.add("ControlAll");
    }

    std::string UartMessageSender::sendMessage()
    {
        std::string buf;
        serializeMsgPack(_jsonDoc, buf);
        appendCheckSum(buf);

        Serial.println(buf.c_str());
        // cout << buf << endl;

        return buf;
    }

}; // namespace UartMessageInterface