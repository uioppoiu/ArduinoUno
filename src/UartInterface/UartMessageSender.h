#ifndef _UART_MESSAGE_SENDER_H_
#define _UART_MESSAGE_SENDER_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageSender
    {
    public:
        UartMessageSender(eMessageType messageType, eCommandType commandType);

        // Request
        void appendRequest(eDataType type, const std::string &name); // All 이면 return;
        void appendRequestAll(eDataType type);                  // 나머지 비우고 All로

        // Response
        template<typename VALUE_TYPE>
        void appendResponse(eDataType type, const std::string &name, const VALUE_TYPE &value, eValueType valueType)
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
            default:
                return;
            }
            data["Value"] = value;
        }

        // Subscribe
        void appendSubscribe(eDataType type, const std::string &name, unsigned int period); // All 이면 return;
        void appendSubscribeAll(eDataType type, unsigned int period);                  // 나머지 비우고 All로

        // Unsubscribe
        void appendUnsubscribe(eDataType type, const std::string &name); // All 이면 return;
        void appendUnsubscribeAll(eDataType type);                  // 나머지 비우고 All로

        std::string sendMessage();

    private:
        unsigned int _seqId;
        DynamicJsonDocument _jsonDoc;
        UartMessageSender();
    };

}; // namespace UartMessageInterface

#endif
