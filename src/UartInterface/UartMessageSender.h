#ifndef _UART_MESSAGE_SENDER_H_
#define _UART_MESSAGE_SENDER_H_

#include "ArduinoSTL.h"
#include "system_configuration.h"
#include "unwind-cxx.h"

#include "tinyxml2/tinyxml2.h"
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

using namespace tinyxml2;

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
            XMLElement *child = NULL;
            switch (type)
            {
            case SensorTemperature:
                child = _xmlDoc.NewElement("TEMPERATURE");
                break;
            case SensorCO2:
                child = _xmlDoc.NewElement("CO2");
                break;
            case SensorHumidity:
                child = _xmlDoc.NewElement("HUMIDITY");
                break;
            case SensorConductivity:
                child = _xmlDoc.NewElement("CONDUCTIVITY");
                break;
            case Control0:
                child = _xmlDoc.NewElement("CONTROL0");
                break;
            default:
                return;
            }

            child->SetAttribute("NAME", name.c_str());
            if (valueType == Double)
            {
                child->SetAttribute("TYPE", "DOUBLE");
                child->SetAttribute("VALUE", value);
            }
            else // Integer
            {
                child->SetAttribute("TYPE", "INTEGER");
                child->SetAttribute("VALUE", value);
            }

            _command->InsertEndChild(child);
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
        XMLDocument _xmlDoc;
        XMLElement *_command;
        UartMessageSender();
    };

}; // namespace UartMessageInterface

#endif
