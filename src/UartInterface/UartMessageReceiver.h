#ifndef _UART_MESSAGE_RECEIVER_H_
#define _UART_MESSAGE_RECEIVER_H_


#include <Arduino.h>
#include <ArduinoJson.h>
#include "UartMessageInterface.h"
#include "UartMessageCallbackManagement.h"

namespace UartMessageInterface
{
    class UartMessageReceiver
    {
    public:
        UartMessageReceiver(const std::string &message);
        bool isMessageValid();
        void processMessage();
        
    private:
        DynamicJsonDocument _jsonDoc;
        UartMessageReceiver();

        bool isUartMessage(const std::string &message);
        
        void handleRequestMessage(const XMLElement *root);
        void invokeRequestGet(const XMLElement *element);
        void invokeSubscribe(const XMLElement *element);
        void invokeUnsubscribe(const XMLElement *element);
        void handleResponseMessage(const XMLElement *root);
        void invokeResponseGet(const XMLElement *element);

        // TODO Set
    };

}; // namespace UartMessageInterface

#endif
